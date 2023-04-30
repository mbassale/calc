#pragma once

#include "AST.h"
#include "Lexer.h"
#include "llvm/Support/raw_ostream.h"

class Parser
{
private:
  Lexer &Lex;
  Token Tok;
  bool HasError;

  void error()
  {
    llvm::errs() << "Unexpected token: " << (int)Tok.getKind() << ": " << Tok.getText() << "\n";
    HasError = true;
  }

  void advance() { Lex.next(Tok); }

  bool expect(Token::TokenKind Kind)
  {
    if (!Tok.is(Kind))
    {
      error();
      return true;
    }
    return false;
  }

  bool consume(Token::TokenKind Kind)
  {
    if (expect(Kind))
      return true;
    advance();
    return false;
  }

  AST *parseCalc();
  Expr *parseExpr();
  Expr *parseTerm();
  Expr *parseFunctions();
  Expr *parseFactor();

public:
  Parser(Lexer &Lex) : Lex(Lex), HasError(false)
  {
    advance();
  }

  bool hasError() const { return HasError; }

  AST *parse();
};
