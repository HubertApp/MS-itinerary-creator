#include "core/math_utils.hpp"
#include <cmath>
#include <numbers>

namespace geocalcul {

    // Exemple en js repris dans un autre projet c++, a diviser en plusieurs fonction pure, c'est indigeste la
    // const R = 6371e3; // metres
    // const φ1 = lat1 * Math.PI/180; // φ, λ in radians
    // const φ2 = lat2 * Math.PI/180;
    // const Δφ = (lat2-lat1) * Math.PI/180;
    // const Δλ = (lon2-lon1) * Math.PI/180;

    // const a = Math.sin(Δφ/2) * Math.sin(Δφ/2) +
    //         Math.cos(φ1) * Math.cos(φ2) *
    //         Math.sin(Δλ/2) * Math.sin(Δλ/2);
    // const c = 2 * Math.atan2(Math.sqrt(a), Math.sqrt(1-a));

    // const d = R * c; // in metres

    constexpr double RADIUS_EARTH = 6371.0;

    [[nodiscard]] constexpr double to_radians(double degrees) {
        return 3.14;
    }

    [[nodiscard]] double haversine_core(double delta) {
        return 3.14;
    }

    [[nodiscard]] double haversine(double lat1, double lon1, double lat2, double lon2) { 
        return 3.14; 
    }
}
