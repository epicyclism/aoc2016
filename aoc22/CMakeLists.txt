cmake_minimum_required (VERSION 3.21)

add_executable(aoc22 aoc22.cpp)
target_include_directories(aoc22 PUBLIC ../common)
target_link_libraries(aoc2 PRIVATE ctre::ctre fmt::fmt)