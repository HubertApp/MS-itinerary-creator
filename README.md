# MS-itinerary-creator

## Table des matières

- [Vue d'ensemble](#vue-densemble)
- [Architecture](#architecture)
- [Composants techniques](#composants-techniques)
- [Installation](#installation)
- [Utilisation](#utilisation)
- [Interface gRPC](#interface-grpc)
- [Algorithme A*](#algorithme-a)
- [Déploiement](#déploiement)
- [Tests](#tests)
- [Performance](#performance)
- [Guidelines et bonnes pratiques](#guidelines-et-bonnes-pratiques)
- [Résumé technique](#résumé-technique)
- [Ressources et références](#ressources-et-références)

---

## Vue d'ensemble

**MS-itinerary-creator** est un microservice de calcul d'itinéraire haute performance développé en C++20 utilisant l'algorithme A* avec heuristique de Haversine.

### Caractéristiques principales

- Algorithme A* optimisé pour la recherche de chemin
- Heuristique Haversine pour l'estimation de distance géographique
- gRPC pour la communication inter-services
- Programmation fonctionnelle pure
- Couverture de test ~100 % (TDD - Test Driven Development)
- C++20 avec standards modernes
- Docker pour le déploiement containerisé

### Cas d'usage

Calcul d'itinéraires optimaux entre deux points géographiques dans un graphe routier, avec compensation des imprécisions de l'heuristique par un système de pondération amélioré.

> **(À revoir !!!)**

---

## Architecture

### Principes de conception

#### 1. Test Driven Development (TDD)

Pour ce service, nous avons utilisé la méthode dite du test driven development (TDD). Ainsi les tests unitaires ont été réalisés avant le développement de chaque feature. Cela vise à permettre d'atteindre une couverture de code proche des 100 %. On gagne aussi en assurance quant à la qualité dès l'étape de conception. L'idée est aussi d'assurer une veille de ce qui est le cœur du projet dès même l'étape de développement en récupérant des informations clés, comme la validité mathématique de notre heuristique et plus tard la performance du calcul.

#### 2. Programmation fonctionnelle pure

Nous avons également utilisé le concept de programmation fonctionnelle. Les raisons sont multiples : on avait besoin de quelque chose de déterministe, or en PF, les fonctions sont pures donc déterministes ; par conséquent, on ne s'embête pas à devoir modifier les données d'entrée. Le fait que les fonctions soient pures facilite également notre logique TDD et facilite la création de tests unitaires en petits blocs facilement testables. Par conséquent, la maintenance et le raisonnement sur le code deviennent largement plus faciles et digestes, même pour une personne qui ne connaît pas forcément le C++. Chaque unité de fonction est parfaitement claire et lisible. Cela permet également d'éviter les effets de bord.

Certes, une partie de l'algorithme ne peut être considérée comme du fonctionnel, notamment la double itération du calcul. En théorie cela aurait dû être remanié, à l'instar de ce qui était proposé dans un exemple de A* traduit en programmation fonctionnelle.

> **TODO :** mettre le lien.

Le fait que cela ne soit pas mis en place est un choix stratégique de notre part pour assurer le rendu du livrable à temps, or sa mise en place demandait un plus grand temps de compréhension. Pour autant, nous envisageons, une fois la V1 déployée, d'assurer dans notre roadmap la mise en place totale de la PF dans l'entièreté du microservice pour garder une cohérence et faciliter la maintenance future.

#### 3. Raison du choix fonctionnel

Comme déjà mentionné, cela nous permet de faciliter la maintenance du code complexe qu'est l'algorithme A* et toutes les prochaines variantes et heuristiques que nous envisageons dans le futur. Nous avons espoir  et nous verrons à l'avenir  que cela réduira les bugs liés aux effets de bord, voire les fera entièrement disparaître.

### Structure des répertoires

```
MS-itinerary-creator/
├── include/                          # Fichiers d'en-tête
│   ├── core/
│   │   ├── astar.hpp                # Implémentation A*
│   │   ├── graph.hpp                # Structure de graphe
│   │   ├── priority_queue.hpp       # File de priorité personnalisée
│   │   ├── math_utils.hpp           # Utilitaires mathématiques
│   │   └── exceptions.hpp           # Gestion d'erreurs
│   ├── logging/
│   │   └── logging.hpp              # Système de logging
│   └── astar_service.hpp            # Service gRPC
├── src/
│   ├── engine/                      # Cœur de l'algorithme
│   │   ├── astar.cpp
│   │   ├── graph.cpp
│   │   ├── priority_queue.cpp
│   │   └── math_utils.cpp
│   ├── grpc/
│   │   └── astar_service.cpp        # Implémentation service gRPC
│   └── main.cpp                     # Point d'entrée serveur
├── proto/
│   └── astar.proto                  # Définition interface gRPC
├── tests/                           # Suite de tests unitaires
│   ├── test_astar.cpp
│   ├── test_graph.cpp
│   ├── test_priority_queue.cpp
│   ├── test_math_logique.cpp
│   └── bench_manuel.cpp
├── observability/                   # Configuration monitoring
│   ├── envoy.yaml                   # Configuration proxy Envoy
│   └── prometheus.yml               # Configuration Prometheus
├── CMakeLists.txt                   # Configuration build CMake
├── Dockerfile                       # Image Docker
└── docker-compose.yml               # Orchestration services
```

---

## Composants techniques

### 1. Cœur du moteur (Engine)

#### Graph (`graph.hpp` / `graph.cpp`)

Représentation du graphe routier.

```cpp
struct Node {
  std::string id;      // Identifiant unique
  double lat;          // Latitude
  double lon;          // Longitude
};

struct Edge {
  std::string to_id;   // Nœud destination
  double weight;       // Poids (SECONDES - temps de traversée)
};

struct Graph {
  std::unordered_map<std::string, Node> nodes;
  std::unordered_map<std::string, std::vector<Edge>> adjacency;
};
```

**Fonctions principales :**

- `parse_graph()` : construit le graphe à partir de nœuds et arêtes pour l'inclure dans les `struct` C++
- `find_node()` : recherche un nœud par ID
- `find_neighbors()` : récupère les nœuds adjacents

#### Mathématiques (`math_utils.hpp` / `math_utils.cpp`)

Calculs géométriques et géographiques.

**Heuristique Haversine :**

```
distance = 2 * R * arcsin(sqrt(sin²((lat2-lat1)/2) + cos(lat1)*cos(lat2)*sin²((lon2-lon1)/2)))

Où R = 6371 km (rayon terrestre)
```

Cette heuristique fournit une estimation réaliste mais imprécise. Le système de pondération du microservice `graph-creator` améliore cette précision tout en conservant la performance.

#### Priority Queue (`priority_queue.hpp` / `priority_queue.cpp`)

File de priorité personnalisée (Min-Heap) optimisée pour A*.

**Structure :** `MinHeap = std::vector<std::pair<double, std::string>>`

- `double` : score `f(n) = g(n) + h(n)`
- `string` : identifiant du nœud

**Opérations :**

- `pq_push()` : insertion avec réorganisation
- `pq_pop()` : extraction du minimum
- `pq_empty()` : vérification de vacuité

### 2. Algorithme A* (`astar.hpp` / `astar.cpp`)

#### Fonctionnement général

```
1. INITIALISER open_set avec nœud de départ
2. TANT QUE open_set n'est pas vide :
   a. current ← nœud avec f(n) minimal dans open_set
   b. SI current == goal : RETOURNER chemin
   c. POUR chaque voisin de current :
      - g_tentative = g(current) + poids_arête
      - SI g_tentative < g(voisin) :
        * Mettre à jour came_from[voisin] = current
        * Ajouter voisin à open_set
3. RETOURNER pas de chemin trouvé
```

#### Formules clés

```
g(n)  = coût réel du début jusqu'à n
h(n)  = heuristique estimée de n au but
f(n)  = g(n) + h(n)  [coût estimé du chemin complet via n]
```

#### Structure de résultat

```cpp
struct AStarResult {
  std::vector<std::string> path;      // Chemin (IDs des nœuds)
  double total_cost;                  // Coût total en secondes
  size_t nodes_explored;              // Nombre de nœuds explorés
};
```

#### Signature

```cpp
std::optional<AStarResult> run_astar(
  const Graph &graph,
  const std::string &start,
  const std::string &goal,
  Heuristic h = heuristic_haversine
);
```

### 3. Service gRPC (`astar_service.hpp` / `astar_service.cpp`)

Interface de communication inter-services utilisant gRPC et Protocol Buffers.

---

## Installation

### Prérequis

- Docker (recommandé)
- Optionnel (installation locale) :
  - CMake ≥ 3.14
  - Compilateur C++20 (g++ ou clang++)
  - `libgrpc++-dev`
  - `libprotobuf-dev`
  - `protobuf-compiler`
  - `protobuf-compiler-grpc`

### Installation locale (Linux / macOS / WSL)

> Si vous êtes sur Windows, l'idéal est vraiment d'utiliser une WSL pour ne pas vous embêter avec le paramétrage du compilateur. Pour ce qui est des IDE avec compilateur intégré tel que Code::Blocks, nous n'avons personnellement pas testé ces outils, donc nous ne pourrons pas vous aider en cas de problème.

```bash
# Installer les dépendances
sudo apt update
sudo apt install -y \
  cmake \
  g++ \
  ninja-build \
  git \
  libgrpc++-dev \
  libprotobuf-dev \
  protobuf-compiler \
  protobuf-compiler-grpc

# Cloner le repo
git clone <repo-url>
cd MS-itinerary-creator

# Configuration et build
cmake -G Ninja \
  -DCMAKE_BUILD_TYPE=Release \
  -S . \
  -B build

# Compiler
cmake --build build --parallel $(nproc)

# Binaires générés
# - build/astar_server    (serveur gRPC)
# - build/unit_tests      (tests unitaires)
```

### Installation avec Docker

```bash
cd MS-itinerary-creator

# Construire l'image
docker-compose build

# Lancer le service
docker-compose up -d astar

# Vérifier les logs
docker-compose logs -f astar
```

### Kubernetes

À savoir : nous avons également des pods Kubernetes via minikube qui permettent de démarrer le service. Pour cela, suivre la documentation du projet global ici :

> **TODO :** mettre le lien de la documentation avec K8s et les pods de notre projet.

---

## Utilisation

### Démarrer le serveur

**Localement**

```bash
./build/astar_server
```

Le serveur écoute sur le port **50051** par défaut.

**Avec Docker**

```bash
docker-compose up astar
```

### Appel du service gRPC

#### Exemple en Python (avec `grpcio`)

```python
import grpc
from astar_pb2 import SolveRequest, Node as ProtoNode, Edge as ProtoEdge
from astar_pb2_grpc import AStarServiceStub

# Connexion
channel = grpc.insecure_channel('localhost:50051')
stub = AStarServiceStub(channel)

# Création de nœuds
nodes = [
    ProtoNode(id='A', lat=48.8566, lon=2.3522),  # Paris
    ProtoNode(id='B', lat=48.8626, lon=2.3515),
    ProtoNode(id='C', lat=48.8703, lon=2.3588),
]

# Création d'arêtes (poids en secondes)
edges = [
    ProtoEdge(from_id='A', to_id='B', weight=120.0),  # 2 minutes
    ProtoEdge(from_id='B', to_id='C', weight=90.0),   # 1.5 minute
    ProtoEdge(from_id='A', to_id='C', weight=250.0),  # 4 min 10s
]

# Requête
request = SolveRequest(nodes=nodes, edges=edges, start_id='A', goal_id='C')

# Appel RPC
response = stub.Solve(request)

# Résultat
print(f"Chemin trouvé: {response.found}")
print(f"Chemin: {' -> '.join(response.path)}")
print(f"Coût total: {response.total_cost} secondes")
print(f"Nœuds explorés: {response.nodes_explored}")
```

### Messages Protocol Buffers

**SolveRequest**

```protobuf
message SolveRequest {
  repeated Node nodes    = 1;  // Liste des nœuds du graphe
  repeated Edge edges    = 2;  // Liste des arêtes
  string        start_id = 3;  // ID du nœud de départ
  string        goal_id  = 4;  // ID du nœud d'arrivée
}
```

**SolveResponse**

```protobuf
message SolveResponse {
  bool            found          = 1;  // Chemin trouvé?
  repeated string path           = 2;  // Séquence d'IDs (chemin)
  double          total_cost     = 3;  // Coût total en secondes
  uint64          nodes_explored = 4;  // Nombre de nœuds explorés
}
```

---

## Interface gRPC

### Service : `AStarService`

#### RPC : `Solve`

**Requête : `SolveRequest`**

| Champ | Description |
| --- | --- |
| `nodes` | Liste des nœuds (id, latitude, longitude) |
| `edges` | Liste des arêtes (from_id, to_id, weight) |
| `start_id` | Nœud de départ |
| `goal_id` | Nœud d'arrivée |

**Réponse : `SolveResponse`**

| Champ | Description |
| --- | --- |
| `found` | Booléen indiquant si un chemin a été trouvé |
| `path` | Liste ordonnée d'IDs formant le chemin optimal |
| `total_cost` | Coût total du chemin en secondes |
| `nodes_explored` | Nombre de nœuds explorés (métrique de performance) |

### Contrat

- **Port :** 50051 (par défaut)
- **Protocole :** gRPC / HTTP2
- **Sérialisation :** Protocol Buffers 3

---

## Algorithme A*

### Pseudo-code implémentation

```
function run_astar(graph, start, goal, heuristic):
    if start == goal:
        return {path: [start], cost: 0, explored: 0}

    g_score = {start: 0}
    came_from = {}
    open_set = MinHeap({(h(start, goal), start)})
    explored = 0

    while not open_set.empty():
        current, f_score = open_set.pop_min()

        if current == goal:
            return {
                path: reconstruct_path(came_from, goal),
                cost: g_score[goal],
                explored: explored
            }

        for neighbor in neighbors(graph, current):
            tentative_g = g_score[current] + edge_weight(current, neighbor)
            known_g = g_score.get(neighbor, infinity)

            if tentative_g < known_g:
                came_from[neighbor] = current
                g_score[neighbor] = tentative_g
                f = tentative_g + heuristic(neighbor, goal)
                open_set.add((f, neighbor))

        explored += 1

    return null  // Pas de chemin trouvé
```



### Avantages

- Garantit le chemin optimal si l'heuristique est admissible
- Efficace en pratique avec une bonne heuristique
- Exploration ciblée vers le but

### Limitations

Nous nous sommes contentés de nous limiter à une seule zone ; à l'échelle de la France, nous craignons une consommation mémoire conséquente pour les grands graphes. Nous avons 2 principales pistes que nous souhaitons mettre en place à l'avenir.

#### Piste 1  La méthode « backbone » (colonne vertébrale)

Nous avons identifié un cycle de 3 phases répétées *n* fois au sein de la séquence. C'est-à-dire que ces trois phases sont le cœur d'un calcul d'étape d'un grand itinéraire.

Pour résumer cette méthode, voyons les gares, les arrêts de bus, les ports et aéroports comme étant des **hubs**. Ces hubs permettent de relier deux points extrêmement éloignés sans devoir faire un calcul complexe de tout le réseau routier français : on connaît l'endroit et l'heure d'arrivée et de départ. Cette ligne imaginaire qui relie ces deux points s'appelle **backbone**, une immense colonne vertébrale qui relie les hubs entre eux. Dans cette logique, on réduit une infinité de possibilités (trains intercités multiples, réseaux de bus multiples, à pied, etc.) dont nous savons de toute façon qu'elles ne seront jamais la manière la plus rapide de relier deux hubs éloignés, et on laisse place à un calcul de graphe entre 3 à 4 hubs grand maximum, tels que l'avion, le train ou alors le bus.

De cette manière, le calcul d'itinéraire est divisé en 3 principales phases :

1. **Graphe A  choix du hub :** le microservice `graph-manager`, en fonction de la position GPS, analysera chaque hub pour savoir lequel est le meilleur pour la destination donnée. Une fois le hub validé pour une ville, nous enregistrons le résultat en base de données car généralement, si un hub est meilleur une fois, il le sera 90 % du temps, sauf demande spécifique du user. Cela évitera de refaire le calcul la prochaine fois qu'un usager de cette même ville demandera un trajet similaire.
2. **Graphe B :** calcul de l'itinéraire entre la position du user jusqu'au hub choisi précédemment.
3. **Graphe C :** calcul de l'itinéraire entre le hub d'arrivée et la destination finale.

Dans cette configuration, nous avons splitté le graphe en 3 graphes plus simples et plus légers à appréhender.

Tout à l'heure nous disions que nous avions identifié 3 phases répétées *n* fois au sein de la séquence. Cette répétition *n* correspond au cas où, dans la ville de départ de notre user, aucun hub ne dessert directement notre destination : il faut donc passer par une phase de transit / correspondance. Pour ce faire, notre graphe A va établir une feuille d'étape en cherchant des hubs dans des villes voisines de la nôtre les plus fortement peuplées ; cette ville a plus de chances d'avoir un hub direct ou alors d'avoir des informations sur un itinéraire déjà préparé.

**Cas de figure :** pour aller de Lyon à Bordeaux, il n'y a pas de ligne de TGV directe, il faut d'abord passer par Paris et, de Paris, prendre une correspondance gare de Lyon pour se rendre à Lyon. Voici les étapes que nous envisageons pour notre futur algo :

- **Graphe A :** départ vers hub de Bordeaux
- **Graphe B :** hub de Bordeaux vers hub Paris-Montparnasse
- **Graphe C :** hub Paris-Montparnasse vers hub Paris Gare de Lyon
- **Graphe D :** hub Paris Gare de Lyon vers hub de Lyon
- **Graphe E :** Gare de Lyon vers destination

À noter, dans ce cas de figure précis, il existe certainement un bus BlaBlaBus / FlixBus qui fait un trajet direct sans escale ; notre algo aura donc certainement préféré le bus au train car plus rapide.

#### Piste 2  Redis

Si cette solution n'est toujours pas suffisante, une seconde hypothèse est d'utiliser Redis pour garder le graphe dans un cache et faire les calculs via la manipulation du graphe en base Redis. Le problème, c'est que cela demandera un effort considérable pour changer l'entièreté du code. Dans tous les cas, cette solution est un peu overkill à notre sens puisqu'une division en sous-graphes est certainement une solution viable.

L'heuristique Haversine seule peut être imprécise, mais on espère que notre méthode de pondération comble ce défaut et, dans tous les cas, nous développerons à l'avenir notre propre heuristique.

### Optimisation appliquée

Le microservice `graph-creator` compensait les imprécisions de Haversine par un système de pondération amélioré, conservant la performance tout en améliorant la précision.

---

## Déploiement

### Architecture Docker

Multi-étapes (builder + runner).

**Stage 1 : Builder**

- Image : `ubuntu:24.04`
- Installe compilateurs et dépendances
- Compile le code
- Génère les binaires : `astar_server`, `unit_tests`

**Stage 2 : Runner**

- Image légère : `ubuntu:24.04`
- Copie uniquement les binaires
- Réduit la taille de l'image (élimination des outils de build)
- Expose le port 50051

### Services Docker Compose

**`astar` (service principal)**

- Image : buildée localement
- Port : 50051 (gRPC)
- Variables : `REDIS_HOST`, `REDIS_PORT` (optionnel)

**`envoy` (proxy / reverse proxy)**

- Image : `envoyproxy/envoy:v1.31-latest`
- Ports : 50051 (exposition publique), 9901 (admin)
- Rôle : terminer les connexions gRPC, logging, métriques

**`cadvisor` (monitoring)**

- Image : `gcr.io/cadvisor/cadvisor:v0.49.1`
- Port : 8080
- Rôle : monitoring des ressources conteneurs

### Commandes

```bash
# Démarrer tous les services
docker-compose up -d

# Démarrer service principal uniquement
docker-compose up -d astar

# Exécuter les tests
docker-compose run --rm -p test unit_tests

# Logs
docker-compose logs -f astar

# Arrêter
docker-compose down

# Nettoyer
docker-compose down -v
```

### Configuration Envoy

Le fichier `observability/envoy.yaml` configure :

- Listeners gRPC sur port 50051
- Upstream vers service `astar:50051`
- Logging des requêtes
- Métriques Prometheus

### Variables d'environnement

```bash
REDIS_HOST=redis      # Host Redis (optionnel)
REDIS_PORT=6379       # Port Redis (optionnel)
```

---

## Tests

**Framework :** Google Test (gtest)

### Couverture

~100 % de couverture de code avec tests écrits en TDD.

### Suite de tests

**`test_math_logique.cpp`**  tests des utilitaires mathématiques, notamment :

- Distance Haversine
- Calculs géométriques

**`test_graph.cpp`**  tests du parsing et de la manipulation du graphe :

- Construction du graphe
- Recherche de nœuds
- Détection de voisins

**`test_priority_queue.cpp`**  tests de la file de priorité :

- Insertion / suppression
- Ordre heap
- Opérations sur éléments

**`test_astar.cpp`**  tests de l'algorithme A* :

- Chemins simples
- Cas limites (pas de chemin, départ = arrivée)
- Optimalité du chemin

**`bench_manuel.cpp`**  benchmarks manuels pour évaluation de performance.

### Exécuter les tests

**Localement**

```bash
./build/unit_tests

# Output
[==========] Running XX tests from Y test suites.
[       OK ] test_astar.SimpleGraph (15 ms)
...
[==========] XX tests from Y test suites ran. (XXX ms total)
[  PASSED  ] All tests.
```

**Docker**

```bash
docker-compose run --rm unit_tests
```

### Exemple de test

> **TODO :** mettre un vrai test.

```cpp
TEST(AStarTest, SimplePathTest) {
    // Arrange
    auto nodes = {Node{"A", ...}, Node{"B", ...}};
    auto edges = {Edge{"A", "B", 100}};
    Graph g = parse_graph(nodes, edges);

    // Act
    auto result = run_astar(g, "A", "B");

    // Assert
    ASSERT_TRUE(result.has_value());
    EXPECT_EQ(result->path, std::vector{"A", "B"});
    EXPECT_EQ(result->total_cost, 100.0);
}
```

---

## Performance

### Facteurs influençant la performance

| Facteur | Impact |
| --- | --- |
| Taille du graphe | O(E log V)  nombre d'arêtes et nœuds |
| Qualité heuristique | Réduction exploration (+100x en cas optimal) |
| Facteur branchement | Exponentiel |
| Profondeur chemin | Linéaire sur chemin final |

### Optimisations appliquées

- **C++20 :** `auto`, structured bindings, optimisations `constexpr`
- **Functional programming :** pas de copies inutiles, move semantics
- **MinHeap :** O(log n) pour opérations de priorité
- **Hash maps :** O(1) recherche de nœuds
- **Compilation Release :** optimisations compilateur
- **Branchless :** minimisation des mispredictions CPU

> **???** Je doute que j'aie mis cela en place, normalement c'est de l'assembleur  à revoir dans le code (point « Branchless »).

### Benchmarking

```bash
./build/unit_tests --benchmark_flag=some_value
# ou
docker-compose run --rm unit_tests
```

### Monitoring

**Prometheus**

- Port : 9090
- Config : `observability/prometheus.yml`
- Métriques exposées par Envoy

**cAdvisor**

- Port : 8080
- Dashboard : http://localhost:8080
- Métriques : CPU, mémoire, I/O, réseau

**Métriques gRPC (Envoy)**

- `grpc_requests_total`
- `grpc_request_duration_seconds`
- `grpc_errors_total`

---

## Guidelines et bonnes pratiques

### Coding standards

- [C++ Core Guidelines](https://isocpp.github.io/CppCoreGuidelines/)
- [Google C++ Style Guide](https://google.github.io/styleguide/cppguide.html)
- [SSCIWR C++ guidelines](https://ssciwr.github.io/guidelines/cpp/)

### Vérifications de code

**Linter (clang-format)**

```bash
# Formatter tous les fichiers
find . -iname "*.cpp" -o -iname "*.hpp" | xargs clang-format -i

# Vérifier format
find . -iname "*.cpp" -o -iname "*.hpp" | xargs clang-format --dry-run -Werror
```

### Débogage

```bash
# Compiler en Debug
cmake -DCMAKE_BUILD_TYPE=Debug -S . -B build
cmake --build build

# Déboguer avec gdb
gdb ./build/unit_tests
(gdb) run
(gdb) break main
(gdb) continue
```

---

## Résumé technique

| Aspect | Détail |
| --- | --- |
| Langage | C++20 |
| Algorithme | A* + heuristique Haversine |
| Communication | gRPC / Protocol Buffers 3 |
| Tests | Google Test (TDD  100 % couverture) |
| Conteneur | Docker multi-étapes |
| Orchestration | Docker Compose / Kubernetes |
| Monitoring | Envoy + cAdvisor + Prometheus |
| Performance | O(E log V) avec optimisations C++20 |
| Paradigme | Programmation fonctionnelle pure |

---

## Ressources et références

### Documentation

- https://isocpp.github.io/CppCoreGuidelines/
- https://google.github.io/styleguide/cppguide.html
- https://learn.microsoft.com/fr-fr/cpp/

### Algorithme A*

- https://en.wikipedia.org/wiki/A*_search_algorithm
- https://blog.frankel.ch/imperative-functional-programming/4/

### gRPC & Protocol Buffers

- https://grpc.io/docs/
- https://developers.google.com/protocol-buffers

### Performance C++

- https://www.w3tutorials.net/blog/how-to-use-noexcept-in-c-or-how-does-it-work/
- https://en.cppreference.com/w/cpp/container/priority_queue
- https://www.intel.com/content/www/us/en/content-details/671488/intel-64-and-ia-32-architectures-optimization-reference-manual-volume-1.html

### Testing

- https://google.github.io/googletest/






-----------------------------------------------------------------------





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