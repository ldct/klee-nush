//===-- IntrinsicCleaner.cpp ----------------------------------------------===//
//
//                     The KLEE Symbolic Virtual Machine
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

#include "Passes.h"

#include "klee/Config/config.h"
#include "llvm/Constants.h"
#include "llvm/DerivedTypes.h"
#include "llvm/Function.h"
#include "llvm/InstrTypes.h"
#include "llvm/Instruction.h"
#include "llvm/Instructions.h"
#include "llvm/IntrinsicInst.h"
#if !(LLVM_VERSION_MAJOR == 2 && LLVM_VERSION_MINOR < 7)
#include "llvm/LLVMContext.h"
#endif
#include "llvm/Module.h"
#include "llvm/Pass.h"
#include "llvm/Analysis/RegionInfo.h"
#include "llvm/Type.h"
#include "llvm/Transforms/Scalar.h"
#include "llvm/Transforms/Utils/BasicBlockUtils.h"
#include "llvm/Target/TargetData.h"
#include "llvm/Support/CFG.h"

#include <iostream>
#include <map>
#include <vector>

using namespace llvm;

namespace klee {

char rewriteReturnPass::ID;

void rewriteReturnPass::getAnalysisUsage(AnalysisUsage &AU) const {
}

bool rewriteReturnPass::runOnFunction(llvm::Function &F) {
  if (F.getNameStr().substr(0,3) == "mem") 
    return 1;
  
  BasicBlock* endBB;
  for (Function::iterator i = F.begin(), e = F.end(); i != e; ++i) {
    BasicBlock& BB = *i;
    if (BB.getTerminator()->getOpcode() == Instruction::Ret) {
      endBB = &BB;
      break;
    }
  }
  
  std::cerr << "predecessors of " << endBB->getNameStr() << ": \n";
  
  std::vector<BasicBlock*> preds;
  
  for (pred_iterator PI = pred_begin(endBB), E = pred_end(endBB); PI != E; ++PI) {
    BasicBlock *Pred = *PI;
    preds.push_back(Pred);
  }
  
  for (std::vector<BasicBlock*>::iterator I = preds.begin(), E = preds.end(); I != E; ++I) {
    BasicBlock* BB = *I;
    
    std::cerr << BB->getNameStr() << ":";
    
    BasicBlock::iterator e = BB->end();
    e--;
    e--;
    
    if (!(isa<llvm::StoreInst>(e))) {
      std::cerr << "NOOOOOOOOO\n";
      return 0;
    }
    llvm::StoreInst* SI = cast<llvm::StoreInst>(e);
    
    Value* retVal = SI->getValueOperand();
/*
AllocaInst* instToReplace = ...;
BasicBlock::iterator ii(instToReplace);

ReplaceInstWithInst(instToReplace->getParent()->getInstList(), ii,
                    new AllocaInst(Type::Int32Ty, 0, "ptrToReplacedInt"));
*/

    //e = Pred->back();

    //std::cerr << "replace " << e->getOpcode();
    TerminatorInst* TI = BB->getTerminator();
    std::cerr << "lol " << TI->getOpcodeName() << "\n";
    
    BasicBlock::iterator ii(TI);

    ReplaceInstWithValue(TI->getParent()->getInstList(), ii,
                         ReturnInst::Create(F.getContext(), retVal));

    endBB->removePredecessor(BB);
     
  }
  std::cerr << "BAIS\n";
  
  return 1;
}

}
