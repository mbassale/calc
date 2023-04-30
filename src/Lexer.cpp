#include "Lexer.h"

namespace charinfo
{
  LLVM_READNONE bool isWhitespace(char c)
  {
    return c == ' ' || c == '\t' || c == '\n' || c == '\r' || c == '\v' || c == '\f';
  }

  LLVM_READNONE bool isDigit(char c) { return c >= '0' && c <= '9'; }

  LLVM_READNONE bool isLetter(char c)
  {
    return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z');
  }
} // namespace charinfo

void Lexer::next(Token &token)
{
  while (*BufferPtr && charinfo::isWhitespace(*BufferPtr))
  {
    ++BufferPtr;
  }

  if (!*BufferPtr)
  {
    token.Kind = Token::TokenKind::EOI;
    return;
  }

  if (charinfo::isLetter(*BufferPtr))
  {
    const char *end = BufferPtr + 1;
    while (charinfo::isLetter(*end))
    {
      ++end;
    }
    llvm::StringRef name(BufferPtr, end - BufferPtr);
    Token::TokenKind kind = name == "with" ? Token::TokenKind::WITH : Token::TokenKind::IDENTIFIER;
    formToken(token, end, kind);
    return;
  }
  else if (charinfo::isDigit(*BufferPtr))
  {
    const char *end = BufferPtr + 1;
    while (charinfo::isDigit(*end))
    {
      ++end;
    }
    formToken(token, end, Token::TokenKind::NUMBER);
    return;
  }
  else
  {
    switch (*BufferPtr)
    {
#define CASE(ch, tok)                     \
  case ch:                                \
    formToken(token, BufferPtr + 1, tok); \
    break
      CASE('+', Token::TokenKind::PLUS);
      CASE('-', Token::TokenKind::MINUS);
      CASE('*', Token::TokenKind::STAR);
      CASE('/', Token::TokenKind::SLASH);
      CASE('(', Token::TokenKind::LPAREN);
      CASE(')', Token::TokenKind::RPAREN);
      CASE(':', Token::TokenKind::COLON);
      CASE(',', Token::TokenKind::COMMA);
      CASE('^', Token::TokenKind::POWER);
#undef CASE
    default:
      formToken(token, BufferPtr + 1, Token::TokenKind::UNKNOWN);
      break;
    }
    return;
  }
}

void Lexer::formToken(Token &Tok, const char *TokEnd, Token::TokenKind Kind)
{
  Tok.Kind = Kind;
  Tok.Text = llvm::StringRef(BufferPtr, TokEnd - BufferPtr);
  BufferPtr = TokEnd;
}