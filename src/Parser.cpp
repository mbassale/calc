#include "Parser.h"

Expr *Parser::parseFactor()
{
  Expr *Res = nullptr;
  switch (Tok.getKind())
  {
  case Token::TokenKind::NUMBER:
    Res = new Factor(Factor::Number, Tok.getText());
    advance();
    break;
  case Token::TokenKind::IDENTIFIER:
    Res = new Factor(Factor::Ident, Tok.getText());
    advance();
    break;
  case Token::TokenKind::LPAREN:
    advance();
    Res = parseExpr();
    if (!consume(Token::TokenKind::RPAREN))
      break;
  default:
    if (!Res)
      error();
    while (!Tok.isOneOf(Token::TokenKind::RPAREN,
                        Token::TokenKind::STAR,
                        Token::TokenKind::SLASH,
                        Token::TokenKind::PLUS,
                        Token::TokenKind::MINUS,
                        Token::TokenKind::EOI))
    {
      advance();
    }
  };
  return Res;
}

Expr *Parser::parseFunctions()
{
  Expr *left = parseFactor();
  while (Tok.is(Token::TokenKind::POWER))
  {
    advance();
    Expr *right = parseFactor();
    left = new BinaryOp(BinaryOp::Power, left, right);
  }
  return left;
}

Expr *Parser::parseTerm()
{
  Expr *left = parseFunctions();
  while (Tok.isOneOf(Token::TokenKind::STAR, Token::TokenKind::SLASH))
  {
    BinaryOp::Operator Op = Tok.is(Token::TokenKind::STAR) ? BinaryOp::Mul : BinaryOp::Div;
    advance();
    Expr *right = parseFunctions();
    left = new BinaryOp(Op, left, right);
  }
  return left;
}

Expr *Parser::parseExpr()
{
  Expr *left = parseTerm();
  while (Tok.isOneOf(Token::TokenKind::PLUS, Token::TokenKind::MINUS))
  {
    BinaryOp::Operator Op = Tok.is(Token::TokenKind::PLUS) ? BinaryOp::Plus : BinaryOp::Minus;
    advance();
    Expr *right = parseTerm();
    left = new BinaryOp(Op, left, right);
  }
  return left;
}

AST *Parser::parseCalc()
{
  Expr *E = nullptr;
  llvm::SmallVector<llvm::StringRef, 8> Vars;
  if (Tok.is(Token::TokenKind::WITH))
  {
    advance();
    if (expect(Token::TokenKind::IDENTIFIER))
    {
      goto _error;
    }
    Vars.push_back(Tok.getText());
    advance();
    while (Tok.is(Token::TokenKind::COMMA))
    {
      advance();
      if (expect(Token::TokenKind::IDENTIFIER))
      {
        goto _error;
      }
      Vars.push_back(Tok.getText());
      advance();
    }
    if (consume(Token::TokenKind::COLON))
    {
      goto _error;
    }
  }
  E = parseExpr();
  if (expect(Token::TokenKind::EOI))
  {
    goto _error;
  }
  if (Vars.empty())
  {
    return E;
  }
  else
  {
    return new WithDecl(Vars, E);
  }
_error:
  while (Tok.getKind() != Token::TokenKind::EOI)
  {
    advance();
  }
  return nullptr;
}

AST *Parser::parse()
{
  AST *Res = parseCalc();
  expect(Token::TokenKind::EOI);
  return Res;
}
