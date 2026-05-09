#include "core/math_utils.hpp"
#include <cmath>
#include <numbers>

namespace geocalcul {

[[nodiscard]] double hav(double delta) {
  double sin = std::sin(delta / 2.0);
  return sin * sin;
}

[[nodiscard]] double haversine_param(double lat_rad, double lon_rad,
                                     double dlat_rad, double dlon_rad) {
  double p =
      hav(dlat_rad) + std::cos(lat_rad) * std::cos(lon_rad) * hav(dlon_rad);
  return p;
}

[[nodiscard]] double haversine_angular_calcul(double param) {
  double a = 2.0 * std::atan2(std::sqrt(param), std::sqrt(1.0 - param));
  return a;
}


[[nodiscard]] double haversine(double lat1, double lon1, double lat2,
                               double lon2) {

  const auto radLat1 = geocalcul::to_radians(lat1);
  const auto radLat2 = geocalcul::to_radians(lat2);

  const auto dratLat =
      geocalcul::to_radians(lat2-lat1);
  const auto dratLon =
      geocalcul::to_radians(lon2-lon1);

  const auto param = geocalcul::haversine_param(radLat1, radLat2, dratLat, dratLon);

  const auto angularCalcul = geocalcul::haversine_angular_calcul(param);

  const auto distance = geocalcul::RADIUS_EARTH * angularCalcul;

  return distance;
}
} // namespace geocalcul
