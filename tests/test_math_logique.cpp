#include "core/math_utils.hpp"
#include <cmath>
#include <gtest/gtest.h>
#include <numbers>
#include <type_traits>

static_assert(std::is_same_v<decltype(geocalcul::to_radians(0.0)), double>);

double pi = std::numbers::pi;

TEST(HaversineToRadiansTest, CheckReturnNotNull) {
  double result = geocalcul::to_radians(50.0);
  EXPECT_TRUE(!std::isnan(result))
      << "La fonction devrait retourner un resultat non null/vide";
}

TEST(HaversineToRadiansTest, CheckNotReturnInfiniteValue) {
  double result = geocalcul::to_radians(50.0);
  EXPECT_TRUE(std::isfinite(result))
      << "La fonction devrait retourner resultat fini";
}

TEST(HaversineToRadiansTest, CheckReturnGreatResult) {
  double result = geocalcul::to_radians(180.0);
  ASSERT_TRUE(std::isfinite(result));
  EXPECT_NEAR(result, pi, 1e-9);
}

TEST(HaversineToRadiansTest, ZeroAndNegative) {
  EXPECT_DOUBLE_EQ(geocalcul::to_radians(0.0), 0.0);
  EXPECT_NEAR(geocalcul::to_radians(-90.0), -1.570796, 1e-6);
}

// --------------------------------------------------------------------

TEST(HaversineParamTest, CheckReturnNotNull) {
  double result = geocalcul::haversine_param(1.0, 1.0, 1.0, 1.0);
  EXPECT_TRUE(!std::isnan(result))
      << "La fonction devrait retourner un resultat non null/vide";
}

TEST(HaversineParamTest, CheckNotReturnInfiniteValue) {
  double result = geocalcul::haversine_param(1.0, 1.0, 1.0, 1.0);
  EXPECT_TRUE(std::isfinite(result))
      << "La fonction devrait retourner resultat fini";
}

TEST(HaversineParamTest, CheckReturnDouble) {
  using ReturnType = decltype(geocalcul::haversine_param(1.0, 1.0, 1.0, 1.0));
  bool isDouble = std::is_same<ReturnType, double>::value;
  EXPECT_TRUE(isDouble) << "La fonction devrait retourner un double";
}

TEST(HaversineParamTest, CheckReturnGreatResult) {
  double pi = std::numbers::pi;
  double lat = pi / 6;
  double lon = pi / 3;

  double dlat = pi / 6;
  double dlon = pi / 4;

  EXPECT_NEAR(geocalcul::haversine_param(lat, lon, dlat, dlon),
              0.13040054012994173, 1e-9);
}

TEST(HaversineParamTest, CheckReturnZeroSamePoint) {
  double pi = std::numbers::pi;
  double lat = pi / 4;
  double lon = pi / 4;

  EXPECT_NEAR(geocalcul::haversine_param(lat, lon, 0.0, 0.0), 0.0, 1e-9);
}

TEST(HaversineParamTest, CheckReturnOneNorthToSouthPole) {
  double pi = std::numbers::pi;
  double lat = pi / 2;
  double lon = -pi / 2;
  double dlat = -pi;

  EXPECT_NEAR(geocalcul::haversine_param(lat, -lon, dlat, 0.0), 1.0, 1e-9);
}

// ----------------------------------------------------------------------

TEST(HaversineAngularCalculTest, CheckReturnNotNull) {
  double result = geocalcul::haversine_angular_calcul(1.0);
  EXPECT_TRUE(!std::isnan(result))
      << "La fonction devrait retourner un resultat non null/vide";
}

TEST(HaversineAngularCalculTest, CheckNotReturnInfiniteValue) {
  double result = geocalcul::haversine_angular_calcul(1.0);
  EXPECT_TRUE(std::isfinite(result))
      << "La fonction devrait retourner resultat fini";
}

TEST(HaversineAngularCalculTest, CheckReturnDouble) {
  using ReturnType = decltype(geocalcul::haversine_angular_calcul(1.0));
  bool isDouble = std::is_same<ReturnType, double>::value;
  EXPECT_TRUE(isDouble) << "La fonction devrait retourner un double";
}

TEST(HaversineAngularCalculTest, CheckReturnGreatResult) {
  double pi = std::numbers::pi;
  double param = 0.5;
  EXPECT_NEAR(geocalcul::haversine_angular_calcul(param), pi / 2, 1e-9);
}

TEST(HaversineAngularCalculTest, CheckReturnPiWithParamOne) {
  double pi = std::numbers::pi;
  double param = 1.0;

  EXPECT_NEAR(geocalcul::haversine_angular_calcul(param), pi, 1e-9);
}

TEST(HaversineAngularCalculTest, CheckReturnPiWithParamZeroSamePoint) {
  double param = 0.0;

  EXPECT_NEAR(geocalcul::haversine_angular_calcul(param), 0.0, 1e-9);
}

//-----------------------------------------------------------------------

TEST(HaversineCoreTest, CheckReturnNotNull) {
  double result = geocalcul::hav(50.0);
  EXPECT_TRUE(!std::isnan(result))
      << "La fonction devrait retourner un resultat non null/vide";
}

TEST(HaversineCoreTest, CheckNotReturnInfiniteValue) {
  double result = geocalcul::hav(50.0);
  EXPECT_TRUE(std::isfinite(result))
      << "La fonction devrait retourner resultat fini";
}

TEST(HaversineCoreTest, CheckReturnDouble) {
  using ReturnType = decltype(geocalcul::hav(50.0));
  bool isDouble = std::is_same<ReturnType, double>::value;
  EXPECT_TRUE(isDouble) << "La fonction devrait retourner un double";
}

TEST(HaversineCoreTest, CheckReturnGreatResult) {
  double radian = geocalcul::to_radians(90.0);
  double result = geocalcul::hav(radian);
  ASSERT_TRUE(std::isfinite(result));
  EXPECT_NEAR(result, 0.5, 1e-9) << "La fonction devrait retourner un double";
}

//---------------------------------------------------------------------

TEST(HaversineTest, CheckReturnNotNull) {
  double result = geocalcul::haversine(0.0, 0.0, 0.0, 0.0);
  EXPECT_TRUE(!std::isnan(result))
      << "La fonction devrait retourner un resultat non null/vide";
}

TEST(HaversineTest, CheckNotReturnInfiniteValue) {
  double result = geocalcul::haversine(0.0, 0.0, 0.0, 0.0);
  EXPECT_TRUE(std::isfinite(result))
      << "La fonction devrait retourner resultat fini";
}

TEST(HaversineTest, CheckReturnDouble) {
  using ReturnType = decltype(geocalcul::haversine(0.0, 0.0, 0.0, 0.0));
  bool isDouble = std::is_same<ReturnType, double>::value;
  EXPECT_TRUE(isDouble) << "La fonction devrait retourner un double";
}

TEST(HaversineTest, CheckReturnGreatResult) {
  double lat1 = 49.10600415081308;
  double lon1 = 6.182594836092145;

  double lat2 = 49.10995796261632;
  double lon2 = 6.177141075227262;

  double result = geocalcul::haversine(lat1, lon1, lat2, lon2);

  ASSERT_TRUE(std::isfinite(result));
  EXPECT_NEAR(result, 0.59235843356499385, 1e-9)
      << "La fonction devrait retourner un double";
}

TEST(HaversineTest, CheckReturnZeroWithSamePoint) {
  double lat1 = 49.10600415081308;
  double lon1 = 6.182594836092145;

  double result = geocalcul::haversine(lat1, lon1, lat1, lon1);

  ASSERT_TRUE(std::isfinite(result));
  EXPECT_NEAR(result, 0.0, 1e-9) << "La fonction devrait retourner un 0";
}

TEST(HaversineTest, CheckReturnInverseSymetricPointAR) {
  double lat1 = 49.10600415081308;
  double lon1 = 6.182594836092145;

  double lat2 = 6.182594836092145;
  double lon2 = 49.10600415081308;

  double result = geocalcul::haversine(lat1, lon1, lat2, lon2);
  double resultSymetric = geocalcul ::haversine(lat2, lon2, lat1, lon1);

  ASSERT_TRUE(std::isfinite(result));
  ASSERT_TRUE(std::isfinite(resultSymetric));

  EXPECT_NEAR(result, resultSymetric, 1e-9)
      << "La fonction devrait retourner une distance indentique sur un trajet "
         "aller-retour";
}

TEST(HaversineTest, CheckReturnPiAtAntiPodalPoint) {
  double lat1 = 0.0;
  double lon1 = 0.0;

  double lat2 = 0.0;
  double lon2 = 180.0;

  double result = geocalcul::haversine(lat1, lon1, lat2, lon2);

  ASSERT_TRUE(std::isfinite(result));
  EXPECT_NEAR(result, std::numbers::pi * geocalcul::RADIUS_EARTH, 1.0)
      << "La fonction devrait retourner une distance indentique sur un trajet "
         "aller-retour";
}
