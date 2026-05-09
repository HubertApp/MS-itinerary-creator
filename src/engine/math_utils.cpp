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

// Exemple en js repris dans un autre projet c++, a diviser en plusieurs
// fonction pure, c'est indigeste la const R = 6371e3; // metres const φ1 = lat1
// * Math.PI/180; // φ, λ in radians const φ2 = lat2 * Math.PI/180; const Δφ =
// (lat2-lat1) * Math.PI/180; const Δλ = (lon2-lon1) * Math.PI/180;

// const a = Math.sin(Δφ/2) * Math.sin(Δφ/2) +
//         Math.cos(φ1) * Math.cos(φ2) *
//         Math.sin(Δλ/2) * Math.sin(Δλ/2);
// const c = 2 * Math.atan2(Math.sqrt(a), Math.sqrt(1-a));

// const d = R * c; // in metres

[[nodiscard]] double haversine(double lat1, double lon1, double lat2,
                               double lon2) {

  double radLat1 = geocalcul::to_radians(lat1);
  double radLat2 = geocalcul::to_radians(lat2);

  double dratLat =
      geocalcul::to_radians(geocalcul::haversine_soustraction(lat2, lat1));
  double dratLon =
      geocalcul::to_radians(geocalcul::haversine_soustraction(lon2, lon1));

  double param = geocalcul::haversine_param(radLat1, radLat2, dratLat, dratLon);

  double angularCalcul = geocalcul::haversine_angular_calcul(param);

  double distance = geocalcul::RADIUS_EARTH * angularCalcul;

  return distance;
}
} // namespace geocalcul
