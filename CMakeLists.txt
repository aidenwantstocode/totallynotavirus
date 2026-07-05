cmake_minimum_required(VERSION 3.11)
project(totallynotavirus VERSION 1.0)

find_package(SFML 2.6 COMPONENTS graphics window audio network system REQUIRED)

include_directories(include)

file(GLOB_RECURSE SRC_FILES "src/*.cpp")
add_executable(totallynotavirus ${SRC_FILES})

target_link_libraries(totallynotavirus sfml-graphics sfml-window sfml-audio sfml-network sfml-system)