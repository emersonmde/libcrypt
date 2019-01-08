cmake_minimum_required(VERSION 3.12)
project(libcrypto C)

set(CMAKE_C_STANDARD 11)

include_directories(include)

add_subdirectory(lib)
add_subdirectory(src)

