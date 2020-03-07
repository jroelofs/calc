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
  EXPECT_EQ(P.parse(), 3);
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
  EXPECT_EQ(P.parse(), 5);
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
    {"1 + 1 ", 2},
    {"4/2", 2}
  };
  // clang-format on

  for (const auto &v : vec) {
    std::stringstream SS(v.input);
    IOSLexer L(SS);
    Parser<int> P(L);
    EXPECT_EQ(P.parse(), v.expected) << v.input;
  }
}

TEST(Eval, Floats) {
  std::stringstream SS("3/2");
  IOSLexer L(SS);
  Parser<float> P(L);
  EXPECT_EQ(P.parse(), 1.5f);
}

TEST(Eval, ParseErrors) {
  // clang-format off
  struct {
    const char *input;
    const char *message;
  } vec[] = {
    { "a", "expected number" },
    { "1 + a", "expected number" },
    { "(1))", "unexpected trailing characters" },
  };
  // clang-format on

  for (const auto &v : vec) {
    std::stringstream SS(v.input);
    IOSLexer L(SS);
    Parser<int> P(L);
    ErrorOr<int> Res = P.parse();
    EXPECT_TRUE(Res.hasError()) << v.input;
    EXPECT_EQ(Res.getError().Msg, v.message) << v.input;
  }
}
