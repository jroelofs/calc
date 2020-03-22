#include "calc/Lexer.h"
#include "calc/Parser.h"
#include "gtest/gtest.h"

#include <tuple>

using namespace calc;

class EvalSuccessFixture : public ::testing::TestWithParam<std::tuple<const char *, int>> {};
TEST_P(EvalSuccessFixture, Exprs) {
  std::stringstream SS(std::get<0>(GetParam()));
  StreamLexer L(SS);
  Parser<int> P(L);
  ErrorOr<int> Res = P.parse();
  EXPECT_FALSE(Res.hasError()) << Res.getError();
  EXPECT_EQ(Res, std::get<1>(GetParam())) << std::get<0>(GetParam());
}

INSTANTIATE_TEST_SUITE_P(EvalTests, EvalSuccessFixture,
  testing::Values(
    std::make_tuple("1", 1),
    std::make_tuple("(1)", 1),
    std::make_tuple("1+1", 2),
    std::make_tuple("1+2*3", 7),
    std::make_tuple("2*(1+1)", 4),
    std::make_tuple("1 + 1 ", 2),
    std::make_tuple("4/2", 2),
    std::make_tuple("-1", -1),
    std::make_tuple("+1", 1),
    std::make_tuple("1++1", 2),
    std::make_tuple("1+-1", 0),
    std::make_tuple("!!42", 1),
    std::make_tuple("-!+1", 0),
    std::make_tuple("13---+-7", 20),
    std::make_tuple("4/2", 2)
  ));

class EvalFailFixture : public ::testing::TestWithParam<std::tuple<const char *, const char *>> {};
TEST_P(EvalFailFixture, Exprs) {
  std::stringstream SS(std::get<0>(GetParam()));
  StreamLexer L(SS);
  Parser<int> P(L);
  ErrorOr<int> Res = P.parse();
  EXPECT_TRUE(Res.hasError()) << std::get<0>(GetParam());
  EXPECT_EQ(Res.getError().Msg, std::get<1>(GetParam())) << std::get<0>(GetParam());
}

INSTANTIATE_TEST_SUITE_P(EvalTests, EvalFailFixture,
  ::testing::Values(
    std::make_tuple("", "expected number"),
    std::make_tuple("a", "expected number"),
    std::make_tuple("1 + a", "expected number"),
    std::make_tuple("(1))", "unexpected trailing characters"),
    std::make_tuple("a + 2", "expected number"),
    std::make_tuple("(1", "expected )"),
    std::make_tuple("999999999999999999999999999", "number out of range")
  ));

TEST(Eval, Floats) {
  std::stringstream SS("3/2");
  StreamLexer L(SS);
  Parser<float> P(L);
  EXPECT_EQ(P.parse(), 1.5f);
}
