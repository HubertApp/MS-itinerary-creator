# Guidelines
https://ssciwr.github.io/guidelines/cpp/

# MS-itinerary-creator
Microservice de calcul d'itinéraire en C++ via l'algorithme de A* avec heurisitique de Haversine.

Il convient de rappeler que le système de pondération dans le microservice graph-creator à était amélioré pour tenir compte de différente donnée pour améliorer la précision du calcul d'itinéraire malgré le choix d'utiliser une heuristique de Haversine. En réaliter, celle-ci est certes plus rapide mais moins précise, ce nouveau système de ponndération compense cette imprécision tout en conservant la performance de l'algorithme A* (couplé a l'heuristique)

# Architecture

Le microservice a été developper avec la logique de tdd, en effet, les tests ont été écris avant le code, ce qui a permis d'avoir une couverture de code proche de 100% et d'assurer la qualité. 

En plus de la logique de tdd, ce microservice a utiliser les principes de développement de programmation fontionnelle, en effet, les fonctions sont pures donc ne modifie pas les données d'entrée et ne dépend pas de l'état du système, évitant ainsi les effets de bord et facilitant la maintenance du code.

## Pourquoi avoir choisi la programmation fonctionnelle ?

Elle a été choisie pour plusieurs raisons, notamment pour sa capacité à faciliter la maintenance, réduire les bugs et améliorer la lisibilité. En utilisant des fonctions pures, on peut facilement raisonner sur le code qui demander pas mal de réflexion, ce qui est le cas pour l'implémentation de A* tout en évitant les effets de bord.

# Prérequis



# Installation

