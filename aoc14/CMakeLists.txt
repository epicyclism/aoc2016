cmake_minimum_required (VERSION 3.21)

add_executable(aoc14 aoc14.cpp)
target_include_directories(aoc14 PUBLIC ../common)
target_link_libraries(aoc14 PRIVATE fmt::fmt OpenSSL::Crypto)