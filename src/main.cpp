#include <iostream>
#include <memory>
#include <string>
#include <grpcpp/grpcpp.h>

#include "astar_service.hpp" 

void RunServer() {
    std::string server_address("0.0.0.0:50051");

    astar::grpc_service::AStarServiceImpl service;

    grpc::ServerBuilder builder;
    
    builder.AddListeningPort(server_address, grpc::InsecureServerCredentials());
    builder.RegisterService(&service); 

    std::unique_ptr<grpc::Server> server(builder.BuildAndStart());
    
    if (!server) {
        std::cerr << "Erreur critique : Le serveur gRPC n'a pas pu démarrer." << std::endl;
        return;
    }

    std::cout << "Serveur gRPC A* lancé avec succès sur " << server_address << std::endl;
    
    server->Wait();
}

int main() {
    RunServer();
    return 0;
}

// Exemple pour l'utilsation de Store graph (pas oublier de supprimer quand c'est fini)

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


// Exemple pour l'utilsation de Solve graph (pas oublier de supprimer quand c'est fini)

// {
//     "goal_id": "D",
//     "graph_id": "test",
//     "start_id": "A",
//     "use_asm": true
// }