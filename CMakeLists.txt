cmake_minimum_required(VERSION 3.27)
project(StephensRelayServer)

set(CMAKE_CXX_STANDARD 17)

add_executable(StephensRelayServer main.cpp
        RelayService.cpp
        RelayService.h)

include_directories(${/Users/stephen/CLionProjects/StephensRelayServer/lib/boost_1_83_0/})

# Use a relative path to include headers located in a subdirectory of your project
target_include_directories(StephensRelayServer PRIVATE ${CMAKE_SOURCE_DIR}/lib/boost_1_83_0/)

target_include_directories(StephensRelayServer PRIVATE ${CMAKE_SOURCE_DIR}/lib/json-develop/include/)
