#include "calc/Lexer.h"
#include "calc/Parser.h"
#include "gtest/gtest.h"

using namespace calc;

static constexpr SLoc NoLoc = SLoc(-1, -1);

TEST(Eval, MockLexer1) {
  // clang-format off
  VectorLexer L({
    Token(NoLoc, Token::Number, "1"),
    Token(NoLoc, Token::Plus),
    Token(NoLoc, Token::Number, "2")
  });
  // clang-format on

  Parser<int> P(L);
  EXPECT_EQ(P.parse(), 3);
}

TEST(Eval, MockLexer2) {
  // clang-format off
  VectorLexer L({
    Token(NoLoc, Token::Number, "1"),
    Token(NoLoc, Token::Times),
    Token(NoLoc, Token::Number, "2"),
    Token(NoLoc, Token::Plus),
    Token(NoLoc, Token::Number, "3")
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
    {"4/2", 2},
    {"-1", -1},
    {"+1", 1},
    {"1++1", 2},
    {"1+-1", 0},
    {"!!42", 1},
    {"-!+1", 0},
    {"13---+-7", 20},
  };
  // clang-format on

  for (const auto &v : vec) {
    std::stringstream SS(v.input);
    IOSLexer L(SS);
    Parser<int> P(L);
    ErrorOr<int> Res = P.parse();
    if (Res.hasError()) {
      EXPECT_TRUE(false) << Res.getError();
    }
    EXPECT_EQ(Res, v.expected) << v.input;
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
    { "a + 2", "expected number" },
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
