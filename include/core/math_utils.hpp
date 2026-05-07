#ifndef MATH_UTILS_H
#define MATH_UTILS_H
#include <numbers>

namespace geocalcul {
    //Première version a changé après avec directement la structure Node quand j'aurais fini cette parti
    
    // Pour conversion degrés en radiant pour les calculs trigo a répétition et chiant
    [[nodiscard]] constexpr double to_radians(double degrees){
        double resultRadiant = degrees * (std::numbers::pi/180.0);
        return resultRadiant;
    }

    // Petit rappel c'est pour le gros calcul sin²(Δ / 2) que je fais cette fonction
    [[nodiscard]] double haversine_core(double delta);
    
    [[nodiscard]] double haversine(double lat1, double lon1, double lat2, double lon2);


}

#endif