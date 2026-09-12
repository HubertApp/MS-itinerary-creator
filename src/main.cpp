#include <exception>
#include <grpcpp/grpcpp.h>
#include <iostream>
#include <memory>
#include <string>

#include "astar_service.hpp"
#include "logging/logging.hpp"

void RunServer() {
  std::string server_address("0.0.0.0:50051");

  astar::grpc_service::AStarServiceImpl service;

  grpc::ServerBuilder builder;

  builder.AddListeningPort(server_address, grpc::InsecureServerCredentials());
  builder.RegisterService(&service);

  std::unique_ptr<grpc::Server> server(builder.BuildAndStart());

  if (!server) {
    LOG_ERROR("le serveur gRPC n'a pas pu demarrer sur " + server_address +
              " (port deja utilise ?)");
    return;
  }

  LOG_INFO("serveur gRPC A* pret sur " + server_address);

  server->Wait();
}

int main() {

  try {
    RunServer();
  } catch (const std::exception &e) {
    LOG_ERROR(std::string("arret sur exception non rattrapee : ") + e.what());
    return 1;
  } catch (...) {
    LOG_ERROR("arret sur exception non rattrapee de type inconnu");
    return 1;
  }
  return 0;
}

// Exemple pour l'utilsation de Store graph (pas oublier de supprimer quand
// c'est fini)

//  {
//      "edges": [
//          {
//             "from_id": "A",
//              "to_id": "B",
//              "weight": 8
//          },
//          {
//             "from_id": "A",
//              "to_id": "C",
//              "weight": 9
//          },
//          {
//              "from_id": "B",
//              "to_id": "D",
//              "weight": 1
//          },
//          {
//              "from_id": "C",
//              "to_id": "D",
//              "weight": 10
//          }
//      ],
//      "graph_id": "test",
//      "nodes": [
//          {
//             "id": "A",
//             "lat": 35.27876,
//             "lon": 64.56461
//          },
//          {
//              "id": "B",
//              "lat": 35.28767452225925,
//              "lon": 64.54618702986537
//          },
//          {
//              "id": "C",
//              "lat": 35.289712234371024,
//              "lon": 64.58191713282973
//          },
//          {
//              "id": "D",
//              "lat": 35.27871174716066,
//              "lon": 64.58635327410236
//          }
//     ]
//  }

// Exemple pour l'utilsation de Solve graph (pas oublier de supprimer quand
// c'est fini)

// {
//     "goal_id": "D",
//     "graph_id": "test",
//     "start_id": "A",
//     "use_asm": true
// }