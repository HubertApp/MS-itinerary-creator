#include <gtest/gtest.h>
#include <cmath>
#include <type_traits>
#include "core/math_utils.hpp"

// A supprimer juste pour tester comment les test fonctionne avec google test
int add(int a, int b) { return a + b; }

// ---------------------------------------------------------------------
// A sans doute décaler dans plusieurs fichier séparer, c'est plus long que prévu en découpant tout pas oublier


TEST(HelloWorldTest, BasicAssertion) { EXPECT_EQ(add(1, 1), 2); }

TEST(HelloWorldTest, StringCheck) {
  std::string hello = "Hello";
  EXPECT_EQ(hello, "Hello");
}

// A pas oublier a l'occasion de mocker une donnée unique

static_assert(std::is_same_v<decltype(geocalcul::to_radians(0.0)), double>);

TEST(HaversineToRadiansTest, CheckReturnNotNull){
    double result = geocalcul::to_radians(50.0);
    EXPECT_TRUE(!std::isnan(result)) << "La fonction devrait retourner un resultat non null/vide";
}

TEST(HaversineToRadiansTest, CheckNotReturnInfiniteValue){
    double result = geocalcul::to_radians(50.0);
    EXPECT_TRUE(std::isfinite(result))  << "La fonction devrait retourner resultat fini";
}

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

TEST(HaversineCoreTest, CheckReturnNotNull){
    double result = geocalcul::hav(50.0);
    EXPECT_TRUE(!std::isnan(result)) << "La fonction devrait retourner un resultat non null/vide";
}

TEST(HaversineCoreTest, CheckNotReturnInfiniteValue){
    double result = geocalcul::hav(50.0);
    EXPECT_TRUE(std::isfinite(result))  << "La fonction devrait retourner resultat fini";
}

TEST(HaversineCoreTest, CheckReturnDouble) {
    using ReturnType = decltype(geocalcul::hav(50.0));
    bool isDouble = std::is_same<ReturnType, double>::value;
    EXPECT_TRUE(isDouble) << "La fonction devrait retourner un double";
}

TEST(HaversineCoreTest, CheckReturnGreatResult){
    double radian = geocalcul::to_radians(90.0);
    double result = geocalcul::hav(radian);
    ASSERT_TRUE(std::isfinite(result));
    EXPECT_NEAR(result, 0.5, 1e-9) << "La fonction devrait retourner un double";
}


static_assert(std::is_same_v<decltype(geocalcul::haversine_soustraction(0.0, 0.0)), double>);

TEST(HaversineSoustractionTest, CheckReturnNotNull){
    double result = geocalcul::haversine_soustraction(0.0, 0.0);
    EXPECT_TRUE(!std::isnan(result)) << "La fonction devrait retourner un resultat non null/vide";
}

TEST(HaversineSoustractionTest, CheckNotReturnInfiniteValue){
    double result = geocalcul::haversine_soustraction(0.0, 0.0);
    EXPECT_TRUE(std::isfinite(result))  << "La fonction devrait retourner resultat fini";
}


TEST(HaversineSoustractionTest, CheckReturnGreatResult){
    double result = geocalcul::haversine_soustraction(50.0,30.0);
    EXPECT_EQ(result, 20.0) << "La fonction devrait retourner 20.0";
}


//---------------------------------------------------------------------

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





