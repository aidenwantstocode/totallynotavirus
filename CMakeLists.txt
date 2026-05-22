cmake_minimum_required(VERSION 3.11)
project(totallynotavirus VERSION 1.0)

# Mencari SFML yang barusan kita instal di MSYS2
find_package(SFML 2.5 COMPONENTS graphics window audio network system REQUIRED)

add_executable(totallynotavirus main.cpp)

# PERBAIKAN: Gunakan nama target huruf kecil bawaan MSYS2
target_link_libraries(totallynotavirus sfml-graphics sfml-window sfml-audio sfml-network sfml-system)