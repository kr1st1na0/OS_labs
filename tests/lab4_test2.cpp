#include "gtest/gtest.h"
#include "realizations.hpp"

TEST(fourthLabTest, SinIntegralStaticTest1) {
    float result = SinIntegral(0, PI, 0.01);
    EXPECT_FLOAT_EQ(result, 2);
}

TEST(fourthLabTest, SinIntegralStaticTest2) {
    float result = SinIntegral(0, PI/2, 0.01);
    EXPECT_FLOAT_EQ(result, 0.9999997);
}

TEST(fourthLabTest, SinIntegralStaticTest3) {
    float result = SinIntegral(0, PI/3, 0.01);
    EXPECT_FLOAT_EQ(result, 0.49999979);
}

TEST(fourthLabTest, PrimeCountStaticTest) {
    float result = PrimeCount(0, 10);
    EXPECT_FLOAT_EQ(result, 4);
}

int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}