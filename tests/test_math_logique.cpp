#include <gtest/gtest.h>
#include <cmath>
#include <type_traits>
#include "core/math_utils.hpp"

// A supprimer juste pour tester comment les test fonctionne avec google test
int add(int a, int b) { return a + b; }




TEST(HelloWorldTest, BasicAssertion) { EXPECT_EQ(add(1, 1), 2); }

TEST(HelloWorldTest, StringCheck) {
  std::string hello = "Hello";
  EXPECT_EQ(hello, "Hello");
}

// A pas oublier a l'occasion de mocker une donnée unique

TEST(HaversineToRadiansTest, CheckReturnNotNull){
    double haversineResult = geocalcul::to_radians(180.0);
    EXPECT_TRUE(!std::isnan(haversineResult)) << "La fonction devrait retourner un resultat non null/vide";
}

TEST(HaversineToRadiansTest, CheckNotReturnInfiniteValue){
    double haversineResult = geocalcul::to_radians(180.0);
    EXPECT_TRUE(std::isfinite(haversineResult))  << "La fonction devrait retourner resultat fini";
}

TEST(HaversineToRadiansTest, CheckReturnDouble) {
    using ReturnType = decltype(geocalcul::to_radians(180.0));
    bool isDouble = std::is_same<ReturnType, double>::value;
    EXPECT_TRUE(isDouble) << "La fonction devrait retourner un double";
}

TEST(HaversineCoreTestTest, CheckReturnNotNull){
    double haversineResult = geocalcul::haversine_core(50.0);
    EXPECT_TRUE(!std::isnan(haversineResult)) << "La fonction devrait retourner un resultat non null/vide";
}

TEST(HaversineCoreTest, CheckNotReturnInfiniteValue){
    double haversineResult = geocalcul::haversine_core(50.0);
    EXPECT_TRUE(std::isfinite(haversineResult))  << "La fonction devrait retourner resultat fini";
}

TEST(HaversineCoreTest, CheckReturnDouble) {
    using ReturnType = decltype(geocalcul::haversine_core(50.0));
    bool isDouble = std::is_same<ReturnType, double>::value;
    EXPECT_TRUE(isDouble) << "La fonction devrait retourner un double";
}

TEST(HaversineTest, CheckReturnNotNull){
    double haversineResult = geocalcul::haversine(0.0, 0.0, 0.0, 0.0);
    EXPECT_TRUE(!std::isnan(haversineResult)) << "La fonction devrait retourner un resultat non null/vide";
}

TEST(HaversineTest, CheckNotReturnInfiniteValue){
    double haversineResult = geocalcul::haversine(0.0, 0.0, 0.0, 0.0);
    EXPECT_TRUE(std::isfinite(haversineResult))  << "La fonction devrait retourner resultat fini";
}

TEST(HaversineTest, CheckReturnDouble) {
    using ReturnType = decltype(geocalcul::haversine(0.0, 0.0, 0.0, 0.0));
    bool isDouble = std::is_same<ReturnType, double>::value;
    EXPECT_TRUE(isDouble) << "La fonction devrait retourner un double";
}

