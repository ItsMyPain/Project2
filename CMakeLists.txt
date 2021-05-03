cmake_minimum_required(VERSION 3.19)
project(Project2)

set(CMAKE_CXX_STANDARD 20)
set(SFML_DIR SFML/lib/cmake/SFML)
find_package(SFML 2.5 COMPONENTS graphics audio window network system REQUIRED)

add_executable(Project2 main.cpp)
target_link_libraries(Project2 sfml-graphics sfml-audio sfml-window sfml-network sfml-system)


