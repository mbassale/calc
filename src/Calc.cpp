#include "Lexer.h"
#include "Parser.h"
#include "Sema.h"
#include "llvm/Support/CommandLine.h"
#include "llvm/Support/InitLLVM.h"
#include "llvm/Support/raw_ostream.h"

class ASTPrinter : public ASTVisitor
{
public:
  void visit(AST &ast) override
  {
    llvm::outs() << "AST\n";
  }
  void visit(Expr &expr) override
  {
    llvm::outs() << "Expr\n";
  }
  void visit(Factor &factor) override
  {
    llvm::outs() << "Factor: " << factor.getValue() << "\n";
  }
  void visit(BinaryOp &binOp) override
  {
    llvm::outs() << "BinaryOp: " << (int)binOp.getOp() << "\n";
    auto *left = binOp.getLeft();
    if (left)
    {
      left->accept(*this);
    }
    auto *right = binOp.getRight();
    if (right)
    {
      right->accept(*this);
    }
  }
  void visit(WithDecl &withDecl) override
  {
    llvm::outs() << "WithDecl: ";
    for (auto I = withDecl.begin(), E = withDecl.end(); I != E; ++I)
    {
      llvm::outs() << *I << " ";
    }
    llvm::outs() << "\n";
    withDecl.getExpr()->accept(*this);
  }
};

static llvm::cl::opt<std::string>
    Input(llvm::cl::Positional,
          llvm::cl::desc("<input expression>"),
          llvm::cl::init(""));

int main(int argc, const char **argv)
{
  llvm::InitLLVM X(argc, argv);
  llvm::cl::ParseCommandLineOptions(
      argc, argv, "calc - the expression compiler\n");

  Lexer Lex(Input);
  Token tok;
  Parser Parser(Lex);
  AST *ast = Parser.parse();
  if (Parser.hasError() || !ast)
  {
    llvm::errs() << "Error parsing input\n";
    return 1;
  }
  ASTPrinter astPrinter;
  ast->accept(astPrinter);
  Sema Sema;
  if (Sema.semantic(ast))
  {
    llvm::errs() << "Semantic error\n";
    return 1;
  }
  return 0;
}