#include "Sema.h"
#include "llvm/ADT/StringSet.h"
#include "llvm/Support/raw_ostream.h"

namespace
{
  class DeclCheck : public ASTVisitor
  {
    llvm::StringSet<> Scope;
    bool HasError = false;
    enum ErrorType
    {
      Twice,
      Not
    };

    void error(ErrorType ET, llvm::StringRef V)
    {
      llvm::errs() << "Variable " << V << " "
                   << (ET == Twice ? "declared twice" : "not declared") << "\n";
    }

  public:
    DeclCheck() : HasError(false) {}
    bool hasError() const { return HasError; }

    // Checks if the variable is declared before use.
    virtual void visit(Factor &Node) override
    {
      if (Node.getKind() == Factor::Ident)
      {
        if (Scope.find(Node.getValue()) == Scope.end())
        {
          error(Not, Node.getValue());
        }
      }
    }

    virtual void visit(BinaryOp &Node) override
    {
      if (Node.getLeft())
      {
        Node.getLeft()->accept(*this);
      }
      else
      {
        HasError = true;
      }
      if (Node.getRight())
      {
        Node.getRight()->accept(*this);
      }
      else
      {
        HasError = true;
      }
    }

    virtual void visit(WithDecl &Node) override
    {
      for (auto I = Node.begin(), E = Node.end(); I != E; ++I)
      {
        if (!Scope.insert(*I).second)
        {
          error(Twice, *I);
        }
      }
      if (Node.getExpr())
      {
        Node.getExpr()->accept(*this);
      }
      else
      {
        HasError = true;
      }
    }
  };
}

bool Sema::semantic(AST *Tree)
{
  DeclCheck Check;
  Tree->accept(Check);
  return Check.hasError();
}