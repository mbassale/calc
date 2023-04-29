
#pragma once

#include "llvm/ADT/StringRef.h"
#include "llvm/Support/MemoryBuffer.h"

class Lexer;

class Token
{
public:
  enum class TokenKind : unsigned short
  {
    EOI,
    UNKNOWN,
    IDENTIFIER,
    NUMBER,
    COMMA,
    COLON,
    PLUS,
    MINUS,
    STAR,
    SLASH,
    LPAREN,
    RPAREN,
    WITH
  };

  TokenKind getKind() const { return Kind; }
  llvm::StringRef getText() const { return Text; }

  bool is(TokenKind K) const { return Kind == K; }
  bool isOneOf(TokenKind K1, TokenKind K2) const { return is(K1) || is(K2); }

  template <typename... Ts>
  bool isOneOf(TokenKind K1, TokenKind K2, Ts... Ks) const
  {
    return is(K1) || isOneOf(K2, Ks...);
  }

private:
  friend class Lexer;

  TokenKind Kind;
  llvm::StringRef Text;
};

class Lexer
{
public:
  Lexer(const llvm::StringRef &Buffer)
      : BufferStart(Buffer.begin()), BufferPtr(Buffer.begin()) {}
  void next(Token &token);

private:
  const char *BufferStart;
  const char *BufferPtr;

  void formToken(Token &Tok, const char *TokEnd, Token::TokenKind Kind);
};