#include <gtest/gtest.h>
#include <math_utils.cpp>



int add(int a, int b) {
    return a + b;
}


TEST(HelloWorldTest, BasicAssertion) {
    EXPECT_EQ(add(1, 1), 2);
}

TEST(HelloWorldTest, StringCheck) {
    std::string hello = "Hello";
    EXPECT_EQ(hello, "Hello");
}

// TEST(HaversineTest, CheckReturnFloat) {
//     std::any haversine = haversine();
//     EXPECT_EQ(haversine.type, type.id(float));

// }