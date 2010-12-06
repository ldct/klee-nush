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

#include <iostream>

using namespace llvm;

namespace klee {

char getRegionInfoPass::ID;

void getRegionInfoPass::getAnalysisUsage(AnalysisUsage &AU) const {
  AU.setPreservesAll();
  AU.addRequiredTransitive<RegionInfo>();

}

bool getRegionInfoPass::runOnFunction(llvm::Function &F) {
  std::cerr << F.getNameStr();
  RegionInfo* RI = &getAnalysis<RegionInfo>();
  
  for (Function::iterator i = F.begin(), e = F.end(); i != e; ++i) {
    std::cerr << i->getNameStr() << " " << RI->getRegionFor(i) << RI->getRegionFor(i)->getParent() << "\n";
  }
  
  return 1;
}

}
