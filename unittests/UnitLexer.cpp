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

TEST(Lexer, MockLexer3) {
  VectorLexer L{};

  Parser<int> P(L);
  ErrorOr<int> E = P.parse();
  ASSERT_TRUE(E.hasError()) << *E;
  EXPECT_EQ(E.getError().Msg, "expected number");
}

struct LexerTestParam {
  const char *Input;
  std::vector<Token> Expected;
};
std::ostream &operator<<(std::ostream &OS, const LexerTestParam &P) {
  OS << "Input: '" << P.Input << "' Expected:";
  for (const Token &T : P.Expected) {
    OS << " " << T;
  }
  return OS;
}

class LexerFixture : public testing::TestWithParam<LexerTestParam> {};

TEST_P(LexerFixture, Strings) {
  const LexerTestParam &V = GetParam();
  std::stringstream SS(V.Input);
  StreamLexer L(SS);
  for (auto I = V.Expected.begin(), E = V.Expected.end(); I != E; ++I, L.pop()) {
    EXPECT_FALSE(L.empty());
    EXPECT_TRUE(L.peek().has_value());
    EXPECT_EQ(*L.peek(), *I);
  }
}

LexerTestParam LexerTests[] = {
  {"", {}},
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
INSTANTIATE_TEST_SUITE_P(LexerTests,
  LexerFixture,
  testing::ValuesIn(LexerTests));