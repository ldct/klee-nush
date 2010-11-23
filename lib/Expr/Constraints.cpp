//===-- Constraints.cpp ---------------------------------------------------===//
//
//                     The KLEE Symbolic Virtual Machine
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

#include "klee/Constraints.h"

#include "klee/util/ExprPPrinter.h"
#include "klee/util/ExprVisitor.h"
#include "klee/ExprBuilder.h"

#include <iostream>
#include <fstream>
#include <map>

using namespace klee;

class ExprReplaceVisitor : public ExprVisitor {
private:
  ref<Expr> src, dst;

public:
  ExprReplaceVisitor(ref<Expr> _src, ref<Expr> _dst) : src(_src), dst(_dst) {}

  Action visitExpr(const Expr &e) {
    if (e == *src.get()) {
      return Action::changeTo(dst);
    } else {
      return Action::doChildren();
    }
  }

  Action visitExprPost(const Expr &e) {
    if (e == *src.get()) {
      return Action::changeTo(dst);
    } else {
      return Action::doChildren();
    }
  }
};

class ExprReplaceVisitor2 : public ExprVisitor {
private:
  const std::map< ref<Expr>, ref<Expr> > &replacements;

public:
  ExprReplaceVisitor2(const std::map< ref<Expr>, ref<Expr> > &_replacements) 
    : ExprVisitor(true),
      replacements(_replacements) {}

  Action visitExprPost(const Expr &e) {
    std::map< ref<Expr>, ref<Expr> >::const_iterator it =
      replacements.find(ref<Expr>(const_cast<Expr*>(&e)));
    if (it!=replacements.end()) {
      return Action::changeTo(it->second);
    } else {
      return Action::doChildren();
    }
  }
};

bool ConstraintManager::rewriteConstraints(ExprVisitor &visitor) {
  ConstraintManager::constraints_ty old;
  bool changed = false;

  constraints.swap(old);
  for (ConstraintManager::constraints_ty::iterator 
         it = old.begin(), ie = old.end(); it != ie; ++it) {
    ref<Expr> &ce = *it;
    ref<Expr> e = visitor.visit(ce);

    if (e!=ce) {
      addConstraintInternal(e); // enable further reductions
      changed = true;
    } else {
      constraints.push_back(ce);
    }
  }

  return changed;
}

void ConstraintManager::simplifyForValidConstraint(ref<Expr> e) {
  // XXX 
}

void ConstraintManager::doSimplifyConstraints() {
  constraints_ty newConstraints;
  for(constraints_ty::iterator ci = constraints.begin(), ce = constraints.end(); ci != ce; ci++){
    ref<Expr> e = *ci;    
    newConstraints.push_back(simplify(e));
  }
  constraints = newConstraints;
}

ref<Expr> ConstraintManager::simplify(ref<Expr> e) {
  std::ofstream devnull("/dev/null");
  bindings = ExprPPrinter::printSingleExprAndReturnBindings(devnull, e);
  std::set<std::pair<ref<Expr>,bool> > empty;
  return simplifier(e,empty);
}

ref<Expr> ConstraintManager::simplifier(ref<Expr> e,std::set< std::pair<ref<Expr>,bool> > pairs){

  klee::ExprBuilder *builder = createDefaultExprBuilder();

  ref<Expr> T = builder->True();
  ref<Expr> F = builder->False();

  //substitute the SourChicken principle
  if (pairs.end()!=(pairs.find(std::make_pair(e,true)))) return T;//{std::cerr<<"\nIm Xuan Ji! True simplifying " << e <<"!!!\n";return T;}
  if (pairs.end()!=(pairs.find(std::make_pair(e,false)))) return F;//{std::cerr<<"\nIm Xuan Ji! False simplifying " << e <<"!!!\n";return F;}
  
	if(e->getKind()==Expr::And||e->getKind()==Expr::Or||e->getKind()==Expr::Eq){//if e can be split
		ref<Expr> a = e->getKid(0);
		ref<Expr> b = e->getKid(1);
/*
    std::cerr << "\nhello im simplfying\n" 
              << e << "\ne kind = " << e->getKind() 
              << "\na = " << a << "\na kind = " << a->getKind() 
              << "\nb = " << b << "\nb kind = " << b->getKind()
              << "\npairs size = " << pairs.size();
*/              

		bool aInBindings = (bindings.find(a) != bindings.end());
		bool bInBindings = (bindings.find(b) != bindings.end());
		
		bool aChildInBindings = (a->getKind() == Expr::Not) ? (bindings.find(a->getKid(0)) != bindings.end()) : false;
		bool bChildInBindings = (a->getKind() == Expr::Not) ? (bindings.find(b->getKid(0)) != bindings.end()) : false;
    
    std::set< std::pair<ref<Expr>,bool> > pairsp = pairs;
    std::set< std::pair<ref<Expr>,bool> > pairsq = pairs;
    std::set< std::pair<ref<Expr>,bool> > pairsr = pairs;
    
    /*
    std::cerr << "\naInBindings = " << aInBindings
              << "\nbInBindings = " << bInBindings
              << "\ne kind =" << e->getKind() 
              << "\nkthxbai\n";    
    */          
		if(e->getKind()==Expr::Or && aInBindings) {//make all a in b into false		
			pairsp.insert(std::make_pair(a,false));
			b=simplifier(b,pairsp);
		}
		else if(e->getKind()==Expr::And && aInBindings) {//make all a in b into true		
			pairsp.insert(std::make_pair(a,true));
			b=simplifier(b,pairsp);
		}
		else if(e->getKind()==Expr::Or && bInBindings) {//make all b in a into false		
			pairsp.insert(std::make_pair(b,false));
			a=simplifier(a,pairsp);
		}
		else if(e->getKind()==Expr::And && bInBindings) {//make all b in a into true		
			pairsp.insert(std::make_pair(b,true));
			a=simplifier(a,pairsp);
		}
    else if(e->getKind()==Expr::Or && aChildInBindings) {//make all a in b into true
			pairsp.insert(std::make_pair(a,true));
			b=simplifier(b,pairsp);
		}
		else if(e->getKind()==Expr::And && aChildInBindings) {//make all a in b into false
			pairsp.insert(std::make_pair(a,false));
			b=simplifier(b,pairsp);
		}
		else if(e->getKind()==Expr::Or && bChildInBindings) {//make all b in a into true	
			pairsp.insert(std::make_pair(b,true));
			a=simplifier(a,pairsp);
		}
		else if(e->getKind()==Expr::And && bChildInBindings) {//make all b in a into false	
			pairsp.insert(std::make_pair(b,false));
			a=simplifier(a,pairsp);
    }
		else{
			a=simplifier(a,pairsp);
			b=simplifier(b,pairsp);
		}
	  //std::cerr << "doing basic set theory with kind of e= " << e->getKind() <<"\na\n"<<a<<"\nb\n"<<b<<"\n";
		//basic set theory
		if(e->getKind()==Expr::And && (a==F||b==F)) return F;
		if(e->getKind()==Expr::Or && (a==T||b==T)) return T;
		if(e->getKind()==Expr::And && a==T) return b;
		if(e->getKind()==Expr::And && b==T) return a;
		if(e->getKind()==Expr::Or && a==F) return b;
		if(e->getKind()==Expr::Or && b==F) return a;

    if(e->getKind()==Expr::Eq && a==b) return T;//{std::cerr<<"\nXuanji your holy leader\n";return T;}
    if(e->getKind()==Expr::Eq && ((a == T && b == F) 
                                ||(a == F && b == T))) return F;

    if(e->getKind()==Expr::Eq && a==T) return b;
    if(e->getKind()==Expr::Eq && b==T) return a;
    if(e->getKind()==Expr::Eq && a==F) return builder->Not(b);
    if(e->getKind()==Expr::Eq && b==F) return builder->Not(a);
    
    if(e->getKind()==Expr::And) return builder->And(a,b);
    if(e->getKind()==Expr::Or) return builder->Or(a,b);
    //std::cerr << "but failed";
  }
  
	return e;
}

ref<Expr> ConstraintManager::simplifyExpr(ref<Expr> e) const {
  if (isa<ConstantExpr>(e))
    return e;

  std::map< ref<Expr>, ref<Expr> > equalities;
  
  for(std::vector< ref<Expr> >::const_iterator it = constraints.begin(), ie = constraints.end(); it != ie; ++it) {
    const ref<Expr> d = *it;
    //std::cerr<<"rewrite constraint old\n"<<d<<"\n";
  }

  for (ConstraintManager::constraints_ty::const_iterator 
         it = constraints.begin(), ie = constraints.end(); it != ie; ++it) {
    if (const EqExpr *ee = dyn_cast<EqExpr>(*it)) {
      if (isa<ConstantExpr>(ee->left)) {
        equalities.insert(std::make_pair(ee->right,
                                         ee->left));
      } else {
        equalities.insert(std::make_pair(*it,
                                         ConstantExpr::alloc(1, Expr::Bool)));
      }
    } else {
      equalities.insert(std::make_pair(*it,
                                       ConstantExpr::alloc(1, Expr::Bool)));
    }
  }
  
  return ExprReplaceVisitor2(equalities).visit(e);//XXX This is where merging is done
}

void ConstraintManager::addConstraintInternal(ref<Expr> e) {
  // rewrite any known equalities 

  // XXX should profile the effects of this and the overhead.
  // traversing the constraints looking for equalities is hardly the
  // slowest thing we do, but it is probably nicer to have a
  // ConstraintSet ADT which efficiently remembers obvious patterns
  // (byte-constant comparison).

  switch (e->getKind()) {
  case Expr::Constant:
    assert(cast<ConstantExpr>(e)->isTrue() && 
           "attempt to add invalid (false) constraint");
    break;
    
    // split to enable finer grained independence and other optimizations
  case Expr::And: {
    BinaryExpr *be = cast<BinaryExpr>(e);
    addConstraintInternal(be->left);
    addConstraintInternal(be->right);
    break;
  }

  case Expr::Eq: {
    BinaryExpr *be = cast<BinaryExpr>(e);
    if (isa<ConstantExpr>(be->left)) {
      ExprReplaceVisitor visitor(be->right, be->left);
      rewriteConstraints(visitor);
    }
    constraints.push_back(e);
    break;
  }
    
  default:
    constraints.push_back(e);
    break;
  }
}

void ConstraintManager::addConstraint(ref<Expr> e) {
  e = simplifyExpr(e);
  for(std::vector< ref<Expr> >::const_iterator it = constraints.begin(), ie = constraints.end(); it != ie; ++it) {
    const ref<Expr> d = *it;
    //std::cerr<<"rewrite constraint new\n"<<d<<"\n";
  }

  addConstraintInternal(e);
}
