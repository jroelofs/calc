#include "calc/Lexer.h"
#include "calc/Parser.h"
#include "gtest/gtest.h"

using namespace calc;

static constexpr SLoc NoLoc = SLoc(-1, -1);

TEST(Lexer, MockLexer1) {
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

TEST(Lexer, MockLexer2) {
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

TEST(Lexer, IOSLexer) {
  // clang-format off
  struct {
    const char *input;
    std::vector<Token> expected;
  } vec[] = {
    {"1", {
      Token(NoLoc, Token::Number, "1")
    }},
    {"(1)", {
      Token(NoLoc, Token::LParen),
      Token(NoLoc, Token::Number, "1"),
      Token(NoLoc, Token::RParen)
    }},
    {"1 + 1 ", {
      Token(NoLoc, Token::Number, "1"),
      Token(NoLoc, Token::Plus),
      Token(NoLoc, Token::Number, "1")
    }},
  };
  // clang-format on

  for (const auto &v : vec) {
    std::stringstream SS(v.input);
    IOSLexer L(SS);
    for (auto I = v.expected.begin(), E = v.expected.end(); I != E; ++I, L.pop()) {
      EXPECT_FALSE(L.empty());
      EXPECT_TRUE(L.peek().has_value());
      EXPECT_EQ(*L.peek(), *I);
    }
  }
}
