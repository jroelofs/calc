#ifndef CALC_LEXER_H
#define CALC_LEXER_H

#include "calc/Token.h"

#include <initializer_list>
#include <iostream>
#include <vector>

namespace calc {

class Lexer {
public:
  virtual Token peek() const = 0;
  virtual Token pop() = 0;
  virtual bool empty() const = 0;
};

class VectorLexer : public Lexer {
public:
  VectorLexer(std::initializer_list<Token> TS) {
    Toks.insert(Toks.end(), TS.begin(), TS.end());
    Cursor = Toks.begin();
  }
  Token peek() const override;
  Token pop() override;
  bool empty() const override;

  std::vector<Token> Toks;
  decltype(Toks.begin()) Cursor;
};

class IOSLexer : public Lexer {
public:
  IOSLexer(std::iostream &IOS) : IOS(IOS) {}

  Token peek() const override;
  Token pop() override;
  bool empty() const override;

private:
  std::iostream &IOS;
};

} // namespace calc

#endif
