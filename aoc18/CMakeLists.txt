cmake_minimum_required (VERSION 3.21)

add_executable(aoc18 aoc18.cpp)
target_include_directories(aoc18 PUBLIC ../common)
target_link_libraries(aoc18 PRIVATE ctre::ctre fmt::fmt)