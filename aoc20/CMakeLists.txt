cmake_minimum_required (VERSION 3.21)

add_executable(aoc20 aoc20.cpp)
target_include_directories(aoc19 PUBLIC ../common)
target_link_libraries(aoc2 PRIVATE ctre::ctre fmt::fmt)