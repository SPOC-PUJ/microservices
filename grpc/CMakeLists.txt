cmake_minimum_required(VERSION 3.20)
project(grpcserver)


set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
find_package(Protobuf CONFIG REQUIRED)
find_package(gRPC CONFIG REQUIRED)
find_package(Eigen3 3.3 REQUIRED NO_MODULE)

add_library(protolib proto/elproto.proto)
target_link_libraries(protolib gRPC::grpc++)

target_include_directories(protolib PUBLIC ${CMAKE_CURRENT_BINARY_DIR} Eigen3::Eigen)
protobuf_generate(TARGET protolib LANGUAGE cpp)
protobuf_generate(TARGET protolib LANGUAGE grpc GENERATE_EXTENSIONS .grpc.pb.h .grpc.pb.cc PLUGIN "proto-gen-grpc=${grpc_cpp_plugin_location}")

add_executable(grpcserver src/grpcserver.cc)
target_link_libraries(grpcserver protolib)

# Find Eigen3

# Link Eigen3 headers
#target_link_libraries(grpcserver Eigen3::Eigen)