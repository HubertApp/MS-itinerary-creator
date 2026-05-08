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

static_assert(std::is_same_v<decltype(geocalcul::to_radians(0.0)), double>);

TEST(HaversineToRadiansTest, CheckReturnGreatResult) {
    double result = geocalcul::to_radians(180.0);
    ASSERT_TRUE(std::isfinite(result));
    EXPECT_NEAR(result, M_PI, 1e-9); 
}

TEST(HaversineToRadiansTest, ZeroAndNegative) {
    EXPECT_DOUBLE_EQ(geocalcul::to_radians(0.0), 0.0);
    EXPECT_NEAR(geocalcul::to_radians(-90.0), -1.570796, 1e-6);
}

// ----------------------------------------------------------------------

static_assert(std::is_same_v<decltype(geocalcul::haversine_core(0.0)), double>);

TEST(HaversineCoreTestTest, CheckReturnNotNull){
    double result = geocalcul::haversine_core(50.0);
    EXPECT_TRUE(!std::isnan(result)) << "La fonction devrait retourner un resultat non null/vide";
}

TEST(HaversineCoreTest, CheckNotReturnInfiniteValue){
    double result = geocalcul::haversine_core(50.0);
    EXPECT_TRUE(std::isfinite(result))  << "La fonction devrait retourner resultat fini";
}

TEST(HaversineCoreTest, CheckReturnDouble) {
    using ReturnType = decltype(geocalcul::haversine_core(50.0));
    bool isDouble = std::is_same<ReturnType, double>::value;
    EXPECT_TRUE(isDouble) << "La fonction devrait retourner un double";
}

TEST(HaversineCoreTest, CheckReturnGreatResult){
    double radian = geocalcul::to_radians(90.0);
    double result = geocalcul::haversine_core(radian);
    ASSERT_TRUE(std::isfinite(result));
    EXPECT_NEAR(result, 0.5, 1e-9);
}

// ---------------------------------------------------------------------



//---------------------------------------------------------------------

// static_assert(std::is_same_v<decltype(geocalcul::to_radians(0.0)), double>);

TEST(HaversineTest, CheckReturnNotNull){
    double result = geocalcul::haversine(0.0, 0.0, 0.0, 0.0);
    EXPECT_TRUE(!std::isnan(result)) << "La fonction devrait retourner un resultat non null/vide";
}

TEST(HaversineTest, CheckNotReturnInfiniteValue){
    double result = geocalcul::haversine(0.0, 0.0, 0.0, 0.0);
    EXPECT_TRUE(std::isfinite(result))  << "La fonction devrait retourner resultat fini";
}

TEST(HaversineTest, CheckReturnDouble) {
    using ReturnType = decltype(geocalcul::haversine(0.0, 0.0, 0.0, 0.0));
    bool isDouble = std::is_same<ReturnType, double>::value;
    EXPECT_TRUE(isDouble) << "La fonction devrait retourner un double";
}





