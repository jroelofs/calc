#ifndef CALC_LEXER_H
#define CALC_LEXER_H

#include "calc/Token.h"

#include <cassert>
#include <initializer_list>
#include <iostream>
#include <optional>
#include <vector>

namespace calc {

class Lexer {
public:
  virtual std::optional<Token> peek() = 0;
  virtual Token pop() = 0;
  virtual bool empty() const = 0;
  virtual SLoc location() const = 0;
  virtual ~Lexer();
};

class VectorLexer : public Lexer {
public:
  VectorLexer(std::initializer_list<Token> TS) {
    Toks.insert(Toks.end(), TS.begin(), TS.end());
    Cursor = Toks.begin();
  }
  std::optional<Token> peek() override;
  Token pop() override;
  bool empty() const override;
  virtual SLoc location() const override { return std::make_pair(0, 0); }

  std::optional<Token> Tok;
  std::vector<Token> Toks;
  decltype(Toks.begin()) Cursor;
};

class IOSLexer : public Lexer {
public:
  IOSLexer(std::istream &IS) : IS(IS), Tok(), Line(0), Col(0) {}

  std::optional<Token> peek() override;
  Token pop() override;
  bool empty() const override;
  virtual SLoc location() const override;

protected:
  std::optional<Token> next();

private:
  std::optional<Token> Tok;
  std::istream &IS;
  int Line, Col;
};

} // namespace calc

#endif
