#ifndef MATH_UTILS_H
#define MATH_UTILS_H
#include <cmath>
#include <numbers>

namespace geocalcul {

constexpr double RADIUS_EARTH = 6371.0;

[[nodiscard]] constexpr double to_radians(double degrees) {
  const auto resultRadiant = degrees * (std::numbers::pi / 180.0);
  return resultRadiant;
}

[[nodiscard]] double hav(double delta);

[[nodiscard]] double haversine_param(double lat_rad, double lon_rad,
                                     double dlat_rad, double dlon_rad);

[[nodiscard]] double haversine_angular_calcul(double param);

[[nodiscard]] double haversine(double lat1, double lon1, double lat2,
                               double lon2);

} // namespace geocalcul

#endif