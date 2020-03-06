#include "gtest/gtest.h"

#include "calc/Lexer.h"
#include "calc/Parser.h"

using namespace calc;

TEST(Eval, MockLexer1) {
  // clang-format off
  VectorLexer L({
    Token(Token::Number, "1"),
    Token::Plus,
    Token(Token::Number, "2")
  });
  // clang-format on

  Parser<int> P(L);
  EXPECT_EQ(P.parseExpr(), 3);
}

TEST(Eval, MockLexer2) {
  // clang-format off
  VectorLexer L({
    Token(Token::Number, "1"),
    Token::Times,
    Token(Token::Number, "2"),
    Token::Plus,
    Token(Token::Number, "3")
  });
  // clang-format on

  Parser<int> P(L);
  EXPECT_EQ(P.parseExpr(), 5);
}

TEST(Eval, Expressions) {
  // clang-format off
  struct {
    const char *input;
    int expected;
  } vec[] = {
    {"1", 1},
    {"(1)", 1},
    {"1+1", 2},
    {"1+2*3", 7},
    {"2*(1+1)", 4},
    {"1 + 1", 2}
  };
  // clang-format on

  for (const auto &v : vec) {
    std::stringstream SS(v.input);
    IOSLexer L(SS);
    Parser<int> P(L);
    EXPECT_EQ(P.parseExpr(), v.expected) << v.input;
  }
}
