cmake_minimum_required(VERSION 3.20)
project(myserver)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Find Protobuf and gRPC
find_package(Protobuf CONFIG REQUIRED)
find_package(gRPC CONFIG REQUIRED)

# Add protolib
add_library(protolib proto/signal_first_difference.proto)
target_link_libraries(protolib gRPC::grpc++)
target_include_directories(protolib PUBLIC ${CMAKE_CURRENT_BINARY_DIR})

# Generate protobuf and gRPC code
get_target_property(grpc_cpp_plugin_location gRPC::grpc_cpp_plugin LOCATION)
protobuf_generate(TARGET protolib LANGUAGE cpp)
protobuf_generate(TARGET protolib LANGUAGE grpc
GENERATE_EXTENSIONS .grpc.pb.h .grpc.pb.cc
PLUGIN "protoc-gen-grpc=${grpc_cpp_plugin_location}")

# Add server executable
add_executable(server src/server.cc)
target_link_libraries(server protolib gRPC::grpc++)

