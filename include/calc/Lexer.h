#ifndef CALC_LEXER_H
#define CALC_LEXER_H

#include "calc/Token.h"

#include <initializer_list>
#include <iostream>
#include <optional>
#include <vector>

namespace calc {

class Lexer {
public:
  virtual const Token &peek() const = 0;
  virtual Token pop() = 0;
  virtual bool empty() const = 0;
};

class VectorLexer : public Lexer {
public:
  VectorLexer(std::initializer_list<Token> TS) {
    Toks.insert(Toks.end(), TS.begin(), TS.end());
    Cursor = Toks.begin();
  }
  const Token &peek() const override;
  Token pop() override;
  bool empty() const override;

  std::vector<Token> Toks;
  decltype(Toks.begin()) Cursor;
};

class IOSLexer : public Lexer {
public:
  IOSLexer(std::istream &IS)
    : IS(IS), Tok(Token::Unknown) {
    Tok = next();
  }

  const Token &peek() const override;
  Token pop() override;
  bool empty() const override;

protected:
  std::optional<Token> next();

private:
  std::optional<Token> Tok;
  std::istream &IS;
};

} // namespace calc

#endif
