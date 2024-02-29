cmake_minimum_required(VERSION 3.10)

project(Game)

find_package(SDL2 REQUIRED)

add_executable(Game main.cpp)

target_include_directories(Game PRIVATE ${SDL2_INCLUDE_DIRS})
target_link_libraries(Game PRIVATE ${SDL2_LIBRARIES})