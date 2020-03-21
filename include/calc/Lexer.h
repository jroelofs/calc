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
  Lexer() noexcept {}
  Lexer &operator=(const Lexer &) = delete;
  Lexer(const Lexer &) = delete;
  virtual ~Lexer();

  [[nodiscard]] std::optional<Token> peek() { return doPeek(); }
  Token pop() { return doPop(); }
  [[nodiscard]] bool empty() const { return isEmpty(); }
  [[nodiscard]] SLoc location() const { return getLocation(); }

protected:
  virtual std::optional<Token> doPeek() = 0;
  virtual Token doPop() = 0;
  virtual bool isEmpty() const = 0;
  virtual SLoc getLocation() const = 0;
};

class VectorLexer : public Lexer {
public:
  VectorLexer(std::initializer_list<Token> TS) noexcept
    : Toks{TS.begin(), TS.end()}, Cursor{0}
  {}

protected:
  std::optional<Token> doPeek() override;
  Token doPop() override;
  bool isEmpty() const override;
  SLoc getLocation() const override { return std::make_pair(0, 0); }

private:
  std::optional<Token> Tok;
  std::vector<Token> Toks;
  size_t Cursor;
};

class StreamLexer : public Lexer {
public:
  StreamLexer(std::istream &IS) noexcept : Tok(), IS(IS), Line(0), Col(0) {}
  StreamLexer(const StreamLexer &) = delete;
  StreamLexer &operator=(const StreamLexer &) = delete;

protected:
  std::optional<Token> doPeek() override;
  Token doPop() override;
  bool isEmpty() const override;
  SLoc getLocation() const override;

  std::optional<Token> next();

private:
  std::optional<Token> Tok;
  std::istream &IS;
  int Line;
  int Col;
};

} // namespace calc

#endif
