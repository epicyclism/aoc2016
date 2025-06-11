cmake_minimum_required (VERSION 3.21)

add_executable(aoc24 aoc24.cpp)
target_include_directories(aoc24 PUBLIC ../common)
target_link_libraries(aoc2 PRIVATE ctre::ctre fmt::fmt)