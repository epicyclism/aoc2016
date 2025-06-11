cmake_minimum_required (VERSION 3.21)

add_executable(aoc11 aoc11.cpp)
target_include_directories(aoc11 PUBLIC ../common)
target_link_libraries(aoc2 PRIVATE ctre::ctre fmt::fmt)