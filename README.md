# Guidelines et ressources
https://ssciwr.github.io/guidelines/cpp/
https://blog.frankel.ch/imperative-functional-programming/4/

https://isocpp.github.io/CppCoreGuidelines/CppCoreGuidelines
https://google.github.io/styleguide/cppguide.html

Débat sur l'utilisation ou non du noexpect pour améliorer performance :
https://www.w3tutorials.net/blog/how-to-use-noexcept-in-c-or-how-does-it-work/
https://stackoverflow.com/questions/31826218/noexcept-practice-for-style-and-performance
https://news.ycombinator.com/item?id=41163083

https://learn.microsoft.com/fr-fr/cpp/cpp/auto-cpp?view=msvc-170
https://en.cppreference.com/cpp/utility/optional

https://stackoverflow.com/questions/1943276/what-does-do-in-a-c-declaration
https://www.reddit.com/r/learnprogramming/comments/92hzaa/c_initial_value_of_reference_to_nonconst_must_be/


Finalement la performance de l'ajout de noexpect semble négligeable donc bon et dans le pire des cas c'est l'inverse le coût 
est élevé, on va évite de l'utiliser

Branchless éviter misprediction
(retrouver lien docs intel)


# MS-itinerary-creator
Microservice de calcul d'itinéraire en C++ via l'algorithme de A* avec heurisitique de Haversine.

Il convient de rappeler que le système de pondération dans le microservice graph-creator à était amélioré pour tenir compte de différente donnée pour améliorer la précision du calcul d'itinéraire malgré le choix d'utiliser une heuristique de Haversine. En réaliter, celle-ci est certes plus rapide mais moins précise, ce nouveau système de ponndération compense cette imprécision tout en conservant la performance de l'algorithme A* (couplé a l'heuristique)

# Architecture

Le microservice a été developper avec la logique de tdd, en effet, les tests ont été écris avant le code, ce qui a permis d'avoir une couverture de code proche de 100% et d'assurer la qualité. 

En plus de la logique de tdd, ce microservice a utiliser les principes de développement de programmation fontionnelle, en effet, les fonctions sont pures donc ne modifie pas les données d'entrée et ne dépend pas de l'état du système, évitant ainsi les effets de bord et facilitant la maintenance du code.

## Pourquoi avoir choisi la programmation fonctionnelle ?

Elle a été choisie pour plusieurs raisons, notamment pour sa capacité à faciliter la maintenance, réduire les bugs et améliorer la lisibilité. En utilisant des fonctions pures, on peut facilement raisonner sur le code qui demander pas mal de réflexion, ce qui est le cas pour l'implémentation de A* tout en évitant les effets de bord.


# Outils et framework

Test unitaire : GoogleTest
Communication : gRPC


# Prérequis

Docker


# Installation

En local sans docker :

Via Docker :


# Test unitaire aide 

https://www.reddit.com/r/cpp_questions/comments/sobl57/how_to_unit_test/?tl=fr
https://github.com/catchorg/Catch2

Finalement utilisation de google Test pour les test unitaires beaucoup plus compréhensible que la syntaxe de catch2 



## Rappel développement

Vérifier que la division de l'haversine en fonction pur marche bien

Lancer le linter pour chaque fichier récursivement : find . -iname "*.cpp" -o -iname "*.hpp" | xargs clang-format -i

sudo apt update
sudo apt install -y libprotobuf-dev protobuf-compiler libgrpc++-dev protobuf-compiler-grpc

sudo apt install gdb