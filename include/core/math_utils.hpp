#ifndef MATH_UTILS_H
#define MATH_UTILS_H
#include <cmath>
#include <numbers>

namespace geocalcul {

constexpr double RADIUS_EARTH = 6371.0;

[[nodiscard]] constexpr double to_radians(const double degrees) {
  const auto resultRadiant = degrees * (std::numbers::pi / 180.0);
  return resultRadiant;
}

[[nodiscard]] double hav(const double delta);

[[nodiscard]] double haversine_param(const double lat_rad, const double lon_rad,
                                     const double dlat_rad, const double dlon_rad);

[[nodiscard]] double haversine_angular_calcul(double param);

[[nodiscard]] double haversine(const double lat1, const double lon1, const double lat2,
                               const double lon2);

} // namespace geocalcul

#endif