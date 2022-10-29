cmake_minimum_required(VERSION 3.0)
project(Cpp-OrderBook)

set(CMAKE_CXX_STANDARD 17)
add_compile_options(-W -Wall)

add_subdirectory(src)
add_subdirectory(test)
add_subdirectory(quantcup)

