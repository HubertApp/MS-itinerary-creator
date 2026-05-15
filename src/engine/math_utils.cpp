#include "core/math_utils.hpp"
#include <cmath>
#include <numbers>

namespace geocalcul {

[[nodiscard]] double hav(const double delta) {
  const auto sin = std::sin(delta / 2.0);
  return sin * sin;
}

[[nodiscard]] double haversine_param(const double lat_rad, const double lon_rad,
                                     const double dlat_rad, const double dlon_rad) {
  const auto p =
      hav(dlat_rad) + std::cos(lat_rad) * std::cos(lon_rad) * hav(dlon_rad);
  return p;
}

[[nodiscard]] double haversine_angular_calcul(const double param) {
  const auto a = 2.0 * std::atan2(std::sqrt(param), std::sqrt(1.0 - param));
  return a;
}


[[nodiscard]] double haversine(const double lat1, const double lon1, const double lat2,
                               const double lon2) {

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
