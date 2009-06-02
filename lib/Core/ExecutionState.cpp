//===-- ExecutionState.cpp ------------------------------------------------===//
//
//                     The KLEE Symbolic Virtual Machine
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

#include "klee/ExecutionState.h"

#include "klee/Internal/Module/Cell.h"
#include "klee/Internal/Module/InstructionInfoTable.h"
#include "klee/Internal/Module/KInstruction.h"
#include "klee/Internal/Module/KModule.h"

#include "klee/Expr.h"

#include "Memory.h"

#include "llvm/Function.h"
#include "llvm/Support/CommandLine.h"

#include <iostream>
#include <cassert>
#include <map>
#include <set>
#include <stdarg.h>

using namespace llvm;
using namespace klee;

namespace { 
  cl::opt<bool>
  DebugLogStateMerge("debug-log-state-merge");
}

/***/

StackFrame::StackFrame(KInstIterator _caller, KFunction *_kf)
  : caller(_caller), kf(_kf), callPathNode(0), 
    minDistToUncoveredOnReturn(0), varargs(0) {
  locals = new Cell[kf->numRegisters];
}

StackFrame::StackFrame(const StackFrame &s) 
  : caller(s.caller),
    kf(s.kf),
    callPathNode(s.callPathNode),
    allocas(s.allocas),
    minDistToUncoveredOnReturn(s.minDistToUncoveredOnReturn),
    varargs(s.varargs) {
  locals = new Cell[s.kf->numRegisters];
  for (unsigned i=0; i<s.kf->numRegisters; i++)
    locals[i] = s.locals[i];
}

StackFrame::~StackFrame() { 
  delete[] locals; 
}

/***/

ExecutionState::ExecutionState(KFunction *kf) 
  : fakeState(false),
    underConstrained(false),
    depth(0),
    pc(kf->instructions),
    prevPC(pc),
    queryCost(0.), 
    weight(1),
    instsSinceCovNew(0),
    coveredNew(false),
    forkDisabled(false),
    ptreeNode(0) {
  pushFrame(0, kf);
}

ExecutionState::ExecutionState(const std::vector<ref<Expr> > &assumptions) 
  : fakeState(true),
    underConstrained(false),
    constraints(assumptions),
    queryCost(0.),
    ptreeNode(0) {
}

ExecutionState::~ExecutionState() {
  while (!stack.empty()) popFrame();
}

ExecutionState *ExecutionState::branch() {
  depth++;

  ExecutionState *falseState = new ExecutionState(*this);
  falseState->coveredNew = false;
  falseState->coveredLines.clear();

  weight *= .5;
  falseState->weight -= weight;

  return falseState;
}

void ExecutionState::pushFrame(KInstIterator caller, KFunction *kf) {
  stack.push_back(StackFrame(caller,kf));
}

void ExecutionState::popFrame() {
  StackFrame &sf = stack.back();
  for (std::vector<const MemoryObject*>::iterator it = sf.allocas.begin(), 
         ie = sf.allocas.end(); it != ie; ++it)
    addressSpace.unbindObject(*it);
  stack.pop_back();
}

///

std::string ExecutionState::getFnAlias(std::string fn) {
  std::map < std::string, std::string >::iterator it = fnAliases.find(fn);
  if (it != fnAliases.end())
    return it->second;
  else return "";
}

void ExecutionState::addFnAlias(std::string old_fn, std::string new_fn) {
  fnAliases[old_fn] = new_fn;
}

void ExecutionState::removeFnAlias(std::string fn) {
  fnAliases.erase(fn);
}

/**/

std::ostream &klee::operator<<(std::ostream &os, const MemoryMap &mm) {
  os << "{";
  MemoryMap::iterator it = mm.begin();
  MemoryMap::iterator ie = mm.end();
  if (it!=ie) {
    os << "MO" << it->first->id << ":" << it->second;
    for (++it; it!=ie; ++it)
      os << ", MO" << it->first->id << ":" << it->second;
  }
  os << "}";
  return os;
}

bool ExecutionState::merge(const ExecutionState &b) {
  if (DebugLogStateMerge)
    llvm::cerr << "-- attempting merge of A:" 
               << this << " with B:" << &b << "--\n";
  if (pc != b.pc)
    return false;

  // XXX is it even possible for these to differ? does it matter? probably
  // implies difference in object states?
  if (symbolics!=b.symbolics)
    return false;

  {
    std::vector<StackFrame>::const_iterator itA = stack.begin();
    std::vector<StackFrame>::const_iterator itB = b.stack.begin();
    while (itA!=stack.end() && itB!=b.stack.end()) {
      // XXX vaargs?
      if (itA->caller!=itB->caller || itA->kf!=itB->kf)
        return false;
      ++itA;
      ++itB;
    }
    if (itA!=stack.end() || itB!=b.stack.end())
      return false;
  }

  std::set< ref<Expr> > aConstraints(constraints.begin(), constraints.end());
  std::set< ref<Expr> > bConstraints(b.constraints.begin(), 
                                     b.constraints.end());
  std::set< ref<Expr> > commonConstraints, aSuffix, bSuffix;
  std::set_intersection(aConstraints.begin(), aConstraints.end(),
                        bConstraints.begin(), bConstraints.end(),
                        std::inserter(commonConstraints, commonConstraints.begin()));
  std::set_difference(aConstraints.begin(), aConstraints.end(),
                      commonConstraints.begin(), commonConstraints.end(),
                      std::inserter(aSuffix, aSuffix.end()));
  std::set_difference(bConstraints.begin(), bConstraints.end(),
                      commonConstraints.begin(), commonConstraints.end(),
                      std::inserter(bSuffix, bSuffix.end()));
  if (DebugLogStateMerge) {
    llvm::cerr << "\tconstraint prefix: [";
    for (std::set< ref<Expr> >::iterator it = commonConstraints.begin(), 
           ie = commonConstraints.end(); it != ie; ++it)
      llvm::cerr << *it << ", ";
    llvm::cerr << "]\n";
    llvm::cerr << "\tA suffix: [";
    for (std::set< ref<Expr> >::iterator it = aSuffix.begin(), 
           ie = aSuffix.end(); it != ie; ++it)
      llvm::cerr << *it << ", ";
    llvm::cerr << "]\n";
    llvm::cerr << "\tB suffix: [";
    for (std::set< ref<Expr> >::iterator it = bSuffix.begin(), 
           ie = bSuffix.end(); it != ie; ++it)
      llvm::cerr << *it << ", ";
    llvm::cerr << "]\n";
  }

  // We cannot merge if addresses would resolve differently in the
  // states. This means:
  // 
  // 1. Any objects created since the branch in either object must
  // have been free'd.
  //
  // 2. We cannot have free'd any pre-existing object in one state
  // and not the other

  if (DebugLogStateMerge) {
    llvm::cerr << "\tchecking object states\n";
    llvm::cerr << "A: " << addressSpace.objects << "\n";
    llvm::cerr << "B: " << b.addressSpace.objects << "\n";
  }
    
  std::set<const MemoryObject*> mutated;
  MemoryMap::iterator ai = addressSpace.objects.begin();
  MemoryMap::iterator bi = b.addressSpace.objects.begin();
  MemoryMap::iterator ae = addressSpace.objects.end();
  MemoryMap::iterator be = b.addressSpace.objects.end();
  for (; ai!=ae && bi!=be; ++ai, ++bi) {
    if (ai->first != bi->first) {
      if (DebugLogStateMerge) {
        if (ai->first < bi->first) {
          llvm::cerr << "\t\tB misses binding for: " << ai->first->id << "\n";
        } else {
          llvm::cerr << "\t\tA misses binding for: " << bi->first->id << "\n";
        }
      }
      return false;
    }
    if (ai->second != bi->second) {
      if (DebugLogStateMerge)
        llvm::cerr << "\t\tmutated: " << ai->first->id << "\n";
      mutated.insert(ai->first);
    }
  }
  if (ai!=ae || bi!=be) {
    if (DebugLogStateMerge)
      llvm::cerr << "\t\tmappings differ\n";
    return false;
  }
  
  // merge stack

  ref<Expr> inA = ConstantExpr::alloc(1, Expr::Bool);
  ref<Expr> inB = ConstantExpr::alloc(1, Expr::Bool);
  for (std::set< ref<Expr> >::iterator it = aSuffix.begin(), 
         ie = aSuffix.end(); it != ie; ++it)
    inA = AndExpr::create(inA, *it);
  for (std::set< ref<Expr> >::iterator it = bSuffix.begin(), 
         ie = bSuffix.end(); it != ie; ++it)
    inB = AndExpr::create(inB, *it);

  // XXX should we have a preference as to which predicate to use?
  // it seems like it can make a difference, even though logically
  // they must contradict each other and so inA => !inB

  std::vector<StackFrame>::iterator itA = stack.begin();
  std::vector<StackFrame>::const_iterator itB = b.stack.begin();
  for (; itA!=stack.end(); ++itA, ++itB) {
    StackFrame &af = *itA;
    const StackFrame &bf = *itB;
    for (unsigned i=0; i<af.kf->numRegisters; i++) {
      ref<Expr> &av = af.locals[i].value;
      const ref<Expr> &bv = bf.locals[i].value;
      if (av.isNull() || bv.isNull()) {
        // if one is null then by implication (we are at same pc)
        // we cannot reuse this local, so just ignore
      } else {
        av = SelectExpr::create(inA, av, bv);
      }
    }
  }

  for (std::set<const MemoryObject*>::iterator it = mutated.begin(), 
         ie = mutated.end(); it != ie; ++it) {
    const MemoryObject *mo = *it;
    const ObjectState *os = addressSpace.findObject(mo);
    const ObjectState *otherOS = b.addressSpace.findObject(mo);
    assert(os && !os->readOnly && 
           "objects mutated but not writable in merging state");
    assert(otherOS);

    ObjectState *wos = addressSpace.getWriteable(mo, os);
    for (unsigned i=0; i<mo->size; i++) {
      ref<Expr> av = wos->read8(i);
      ref<Expr> bv = otherOS->read8(i);
      wos->write(i, SelectExpr::create(inA, av, bv));
    }
  }

  constraints = ConstraintManager();
  for (std::set< ref<Expr> >::iterator it = commonConstraints.begin(), 
         ie = commonConstraints.end(); it != ie; ++it)
    constraints.addConstraint(*it);
  constraints.addConstraint(OrExpr::create(inA, inB));

  return true;
}

/**/

/*
   Used for tainting: create a clone of os that we can revirt to with
   the behavior that all constraints are preserved, but writes are 
   discarded.  When we revirt it will be at the same address.
 */
ObjectState *ExecutionState::cloneObject(ObjectState *os, 
                                         MemoryObject *mo) {
  MemoryMap::iterator it = shadowObjects.find(mo);
  if (it != shadowObjects.end())
    assert(0 && "Cannot exist already!");

  llvm::cerr << "DRE: Inserting a cloned object: " << mo << "\n";
  shadowObjects = shadowObjects.replace(std::make_pair(mo, os));
  os = new ObjectState(*os);
  addressSpace.bindObject(mo, os);
  return os;
}

/***/


ExecutionTraceEvent::ExecutionTraceEvent(ExecutionState& state, 
                                         KInstruction* ki)
  : consecutiveCount(1) 
{
  file = ki->info->file;
  line = ki->info->line;
  funcName = state.stack.back().kf->function->getName();
  stackDepth = state.stack.size();
}

bool ExecutionTraceEvent::ignoreMe() const {
  // ignore all events occurring in certain pesky uclibc files:
  if (file.find("libc/stdio/") != std::string::npos) {
    return true;
  }

  return false;
}

void ExecutionTraceEvent::print(std::ostream &os) const {
  os.width(stackDepth);
  os << ' ';
  printDetails(os);
  os << ' ' << file << ':' << line << ':' << funcName;
  if (consecutiveCount > 1)
    os << " (" << consecutiveCount << "x)\n";
  else
    os << '\n';
}


bool ExecutionTraceEventEquals(ExecutionTraceEvent* e1, ExecutionTraceEvent* e2) {
  // first see if their base class members are identical:
  if (!((e1->file == e2->file) &&
        (e1->line == e2->line) &&
        (e1->funcName == e2->funcName)))
    return false;

  // fairly ugly, but i'm no OOP master, so this is the way i'm
  // doing it for now ... lemme know if there's a cleaner way:
  BranchTraceEvent* be1 = dynamic_cast<BranchTraceEvent*>(e1);
  BranchTraceEvent* be2 = dynamic_cast<BranchTraceEvent*>(e2);
  if (be1 && be2) {
    return ((be1->trueTaken == be2->trueTaken) &&
            (be1->canForkGoBothWays == be2->canForkGoBothWays));
  }

  // don't tolerate duplicates in anything else:
  return false;
}


void BranchTraceEvent::printDetails(std::ostream &os) const {
  os << "BRANCH " << (trueTaken ? "T" : "F") << ' ' <<
        (canForkGoBothWays ? "2-way" : "1-way");
}

void ExecutionTraceManager::addEvent(ExecutionTraceEvent* evt) {
  // don't trace anything before __user_main, except for global events
  if (!hasSeenUserMain) {
    if (evt->funcName == "__user_main") {
      hasSeenUserMain = true;
    }
    else if (evt->funcName != "global_def") {
      return;
    }
  }

  // custom ignore events:
  if (evt->ignoreMe())
    return;

  if (events.size() > 0) {
    // compress consecutive duplicates:
    ExecutionTraceEvent* last = events.back();
    if (ExecutionTraceEventEquals(last, evt)) {
      last->consecutiveCount++;
      return;
    }
  }

  events.push_back(evt);
}

void ExecutionTraceManager::printAllEvents(std::ostream &os) const {
  for (unsigned i = 0; i != events.size(); ++i)
    events[i]->print(os);
}

/***/
