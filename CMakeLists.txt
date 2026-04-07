cmake_minimum_required(VERSION 3.22)
project(AStarService)

set(CMAKE_CXX_STANDARD 20)

find_package(gRPC CONFIG REQUIRED)
find_package(Protobuf CONFIG REQUIRED)
find_package(Catch2 CONFIG REQUIRED)

set(_GRPC_CPP_PLUGIN_EXECUTABLE $<TARGET_FILE:gRPC::grpc_cpp_plugin>)
set(_PROTO_PATH "${CMAKE_CURRENT_SOURCE_DIR}/proto")
set(_PROTO_FILE "${_PROTO_PATH}/astar.proto")


set(PROTO_SRCS "${CMAKE_CURRENT_BINARY_DIR}/astar.pb.cc")
set(PROTO_HDRS "${CMAKE_CURRENT_BINARY_DIR}/astar.pb.h")
set(GRPC_SRCS "${CMAKE_CURRENT_BINARY_DIR}/astar.grpc.pb.cc")
set(GRPC_HDRS "${CMAKE_CURRENT_BINARY_DIR}/astar.grpc.pb.h")

add_custom_command(
    OUTPUT "${PROTO_SRCS}" "${PROTO_HDRS}" "${GRPC_SRCS}" "${GRPC_HDRS}"
    COMMAND protobuf::protoc
    ARGS --cpp_out "${CMAKE_CURRENT_BINARY_DIR}"
         --grpc_out "${CMAKE_CURRENT_BINARY_DIR}"
         --plugin "protoc-gen-grpc=${_GRPC_CPP_PLUGIN_EXECUTABLE}"
         -I "${_PROTO_PATH}"
         "${_PROTO_FILE}"
    DEPENDS "${_PROTO_FILE}"
)

add_library(astar_engine src/engine/astar.cpp)
target_include_directories(astar_engine PUBLIC src/engine)

add_executable(unit_tests tests/test_astar.cpp)
target_link_libraries(unit_tests PRIVATE astar_engine Catch2::Catch2WithMain)

add_executable(astar_server 
    src/main.cpp 
    "${PROTO_SRCS}" 
    "${GRPC_SRCS}"
)
target_link_libraries(astar_server 
    PRIVATE 
    astar_engine 
    gRPC::grpc++ 
    protobuf::libprotobuf
)
target_include_directories(astar_server PRIVATE "${CMAKE_CURRENT_BINARY_DIR}")