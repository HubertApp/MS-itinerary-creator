#include <gtest/gtest.h>
#include <type_traits>
#include "core/math_utils.hpp"

// A supprimer juste pour tester comment les test fonctionne avec google test
int add(int a, int b) { return a + b; }

TEST(HelloWorldTest, BasicAssertion) { EXPECT_EQ(add(1, 1), 2); }

TEST(HelloWorldTest, StringCheck) {
  std::string hello = "Hello";
  EXPECT_EQ(hello, "Hello");
}

TEST(HaversineTest, CheckReturnFloat) {
    using ReturnType = decltype(geocalcul::haversine(0.0, 0.0, 0.0, 0.0));
    bool isDouble = std::is_same<ReturnType, double>::value;
    EXPECT_TRUE(isDouble) << "La fonction devrait retourner un double";
}