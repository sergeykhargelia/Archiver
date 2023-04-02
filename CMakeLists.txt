cmake_minimum_required(VERSION 3.10)
project(hw_02 CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -g -Wall -Wextra -Werror -pedantic")

include_directories(include/ test_include/)

add_library(huffman include/huffman_tree.hpp include/huffman_archiver.hpp include/bits_io.hpp include/huffman_exceptions.hpp 
	src/huffman_tree.cpp src/huffman_archiver.cpp src/bits_io.cpp src/huffman_exceptions.cpp)

add_executable(hw_02 src/main.cpp)
add_executable(hw_02_test test_include/doctest.h test/main_test.cpp test/test_exceptions.cpp test/integration_test.cpp test/tree_test.cpp)

target_link_libraries(hw_02 huffman)
target_link_libraries(hw_02_test huffman)
