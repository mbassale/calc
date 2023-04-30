#pragma once
#pragma once

#include "llvm/ADT/SmallVector.h"
#include "llvm/ADT/StringRef.h"

class AST;
class Expr;
class Factor;
class BinaryOp;
class WithDecl;

class ASTVisitor
{
public:
  virtual void visit(AST &) {}
  virtual void visit(Expr &) {}
  virtual void visit(Factor &) {}
  virtual void visit(BinaryOp &) {}
  virtual void visit(WithDecl &) {}
};

class AST
{
public:
  virtual ~AST() {}
  virtual void accept(ASTVisitor &V) = 0;
};

class Expr : public AST
{
public:
  Expr() {}
};

class Factor : public Expr
{
public:
  enum ValueKind
  {
    Ident,
    Number
  };

private:
  ValueKind Kind;
  llvm::StringRef Val;

public:
  Factor(ValueKind Kind, llvm::StringRef Val) : Kind(Kind), Val(Val) {}

  ValueKind getKind() const { return Kind; }
  llvm::StringRef getValue() const { return Val; }

  virtual void accept(ASTVisitor &V) override
  {
    V.visit(*this);
  }
};

class BinaryOp : public Expr
{
public:
  enum Operator
  {
    Plus,
    Minus,
    Mul,
    Div,
    Power,
  };

private:
  Operator Op;
  Expr *Left, *Right;

public:
  BinaryOp(Operator Op, Expr *Left, Expr *Right) : Op(Op), Left(Left), Right(Right) {}

  Operator getOp() const { return Op; }
  Expr *getLeft() const { return Left; }
  Expr *getRight() const { return Right; }

  virtual void accept(ASTVisitor &V) override
  {
    V.visit(*this);
  }
};

class WithDecl : public AST
{
private:
  using VarVector = llvm::SmallVector<llvm::StringRef, 8>;
  VarVector Vars;
  Expr *E;

public:
  WithDecl(VarVector &Vars, Expr *E) : Vars(Vars), E(E) {}

  VarVector::const_iterator begin() { return Vars.begin(); }
  VarVector::const_iterator end() { return Vars.end(); }
  Expr *getExpr() { return E; }

  virtual void accept(ASTVisitor &V) override
  {
    V.visit(*this);
  }
};