#include "gtest/gtest.h"

#include "calc/ErrorOr.h"
#include <string>

using namespace calc;

TEST(ErrorOr, Construction) {
    ErrorOr<int> E1(1234);
    EXPECT_FALSE(E1.hasError());

    ErrorOr<std::string> E2("Houston, we don't have a problem.");
    EXPECT_FALSE(E2.hasError());

    ErrorOr<int> E3 = makeError("He's dead, Jim.");
    EXPECT_TRUE(E3.hasError());
}

TEST(ErrorOr, OperatorBool) {
    if (ErrorOr<int> E1 = ErrorOr(42)) {
        EXPECT_EQ(*E1, 42);
        return;
    }
    FAIL() << "bool conversion broken?";
}

TEST(ErrorOr, OperatorMath) {
    ErrorOr<int> Three = 3;
    ErrorOr<int> Five = 5;
    ErrorOr<int> Err1 = makeError("error1");
    ErrorOr<int> Err2 = makeError("error2");

    EXPECT_EQ(15, Three * Five);
    EXPECT_EQ(8, Three + Five);
    EXPECT_EQ(Err1, Three + Err1);
    EXPECT_EQ(Err1, Err1 + Three);
    EXPECT_EQ(Err1, Err1 + Err2);
    EXPECT_EQ(Err2, Err2 + Err1);
}