set(binary ${CMAKE_PROJECT_NAME})

set(sources rpc_channel.cpp rpc_controller.cpp rpc_meta.pb.cc rpc_server.cpp)

# add_executable(${binary}_exe ${sources})
add_library(${binary}_lib STATIC ${sources})