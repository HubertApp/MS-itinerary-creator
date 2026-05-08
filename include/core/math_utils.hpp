#ifndef MATH_UTILS_H
#define MATH_UTILS_H
#include <cmath>
#include <numbers>

namespace geocalcul {
    //Première version a changé après avec directement la structure Node quand j'aurais fini cette parti
    
    constexpr double RADIUS_EARTH = 6371.0;

    // Pour conversion degrés en radiant pour les calculs trigo a répétition et chiant
    [[nodiscard]] constexpr double to_radians(double degrees){
        double resultRadiant = degrees * (std::numbers::pi/180.0);
        return resultRadiant;
    }

    // Petit rappel c'est pour le gros calcul sin²(Δ / 2) que je fais cette fonction
    [[nodiscard]] constexpr double haversine_core(double delta) {
        double resultSinus = std::sin(delta/2.0);
        return resultSinus * resultSinus;
    }

    [[nodiscard]] constexpr double haversine_soustraction(double l1, double l2){
        return 3.14;
    }

    [[nodiscard]] constexpr double haversine_angular_calcul(){
        return 3.14;
    }
    
    [[nodiscard]] double haversine(double lat1, double lon1, double lat2, double lon2);


}

#endif