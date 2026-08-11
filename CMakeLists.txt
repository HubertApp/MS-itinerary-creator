cmake_minimum_required(VERSION 3.14)
project(AStarCore)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

enable_language(ASM)

find_package(Protobuf REQUIRED)
find_package(gRPC CONFIG REQUIRED)

include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/refs/heads/main.zip
)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

add_library(astar_engine 
    src/engine/math_utils.cpp
    include/core/math_utils.hpp
    src/engine/graph.cpp
    include/core/graph.hpp
    src/engine/priority_queue.cpp
    include/core/priority_queue.hpp
    src/engine/astar.cpp
    include/core/astar.hpp
)
target_include_directories(astar_engine PUBLIC src/engine include)

set(GEN_DIR "${CMAKE_CURRENT_BINARY_DIR}/generated")
file(MAKE_DIRECTORY ${GEN_DIR})

set(PROTO_SRCS "${GEN_DIR}/proto/astar.pb.cc")
set(PROTO_HDRS "${GEN_DIR}/proto/astar.pb.h")
set(GRPC_SRCS "${GEN_DIR}/proto/astar.grpc.pb.cc")
set(GRPC_HDRS "${GEN_DIR}/proto/astar.grpc.pb.h")

add_custom_command(
    OUTPUT ${PROTO_SRCS} ${PROTO_HDRS} ${GRPC_SRCS} ${GRPC_HDRS}
    COMMAND ${Protobuf_PROTOC_EXECUTABLE}
    ARGS --cpp_out=${GEN_DIR}
         --grpc_out=${GEN_DIR}
         --plugin=protoc-gen-grpc=$<TARGET_FILE:gRPC::grpc_cpp_plugin>
         -I ${CMAKE_CURRENT_SOURCE_DIR}
         ${CMAKE_CURRENT_SOURCE_DIR}/proto/astar.proto
    DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/proto/astar.proto
    VERBATIM
)

add_library(astar_grpc_service 
    ${PROTO_SRCS} 
    ${GRPC_SRCS} 
    src/grpc/astar_service.cpp
)

target_include_directories(astar_grpc_service PUBLIC
    include 
    src 
    ${GEN_DIR}
)
    
target_link_libraries(astar_grpc_service PUBLIC
    astar_engine 
    gRPC::grpc++ 
    protobuf::libprotobuf
)


add_executable(astar_server src/main.cpp)
target_link_libraries(astar_server PRIVATE astar_grpc_service)

add_executable(unit_tests 
    tests/test_math_logique.cpp
    tests/test_graph.cpp
    tests/test_priority_queue.cpp
    tests/test_astar.cpp
    # tests/bench_manuel.cpp
)
target_link_libraries(unit_tests PRIVATE astar_engine gtest_main)

include(GoogleTest)
gtest_discover_tests(unit_tests)