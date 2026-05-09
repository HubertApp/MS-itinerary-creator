#ifndef MATH_UTILS_H
#define MATH_UTILS_H
#include <cmath>
#include <numbers>

namespace geocalcul {
    //Première version a changé après avec directement la structure Node quand j'aurais fini cette parti
    
    constexpr double RADIUS_EARTH = 6371.0;

    // Pour conversion degrés en radiant pour les calculs trigo a répétition et chiant
    // Conversion latitude longitude d'un plan 2d à une l'échelle sphérique proche de celle de la terre
    [[nodiscard]] constexpr double to_radians(double degrees){
        double resultRadiant = degrees * (std::numbers::pi/180.0);
        return resultRadiant;
    }

    // Petit rappel sin²(Δ / 2) (mathématique hav)
    [[nodiscard]] double hav(double delta);    

    // Overkill, J'aurais pas du faire cette fonction, a voir si je la garde
    [[nodiscard]] constexpr double haversine_soustraction(double l1, double l2){
        return l1-l2;
    }

    [[nodiscard]] double haversine_param(double lat_rad, double lon_rad, double dlat_rad, double dlon_rad);

    [[nodiscard]] double haversine_angular_calcul(double param);
    
    [[nodiscard]] double haversine(double lat1, double lon1, double lat2, double lon2);


}

#endif