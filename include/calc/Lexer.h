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
  Lexer() {}
  Lexer &operator=(const Lexer &) = delete;
  Lexer(const Lexer &) = delete;
  [[nodiscard]] virtual std::optional<Token> peek() = 0;
  virtual Token pop() = 0;
  [[nodiscard]] virtual bool empty() const = 0;
  [[nodiscard]] virtual SLoc location() const = 0;
  virtual ~Lexer();
};

class VectorLexer : public Lexer {
public:
  VectorLexer(std::initializer_list<Token> TS) noexcept
    : Toks{TS.begin(), TS.end()}, Cursor{0}
  {}
  [[nodiscard]] std::optional<Token> peek() override;
  Token pop() override;
  [[nodiscard]] bool empty() const override;
  [[nodiscard]] SLoc location() const override { return std::make_pair(0, 0); }

private:
  std::optional<Token> Tok;
  std::vector<Token> Toks;
  size_t Cursor;
};

class IOSLexer : public Lexer {
public:
  IOSLexer(std::istream &IS) noexcept : IS(IS), Tok(), Line(0), Col(0) {}

  [[nodiscard]] std::optional<Token> peek() override;
  Token pop() override;
  [[nodiscard]] bool empty() const override;
  [[nodiscard]] SLoc location() const override;

protected:
  [[nodiscard]] std::optional<Token> next();

private:
  std::optional<Token> Tok;
  std::istream &IS;
  int Line, Col;
};

} // namespace calc

#endif
