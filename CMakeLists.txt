cmake_minimum_required(VERSION 3.12)
project(libcrypto C)

set(CMAKE_C_STANDARD 11)

add_library(libcrypto SHARED aes.c aes.h msws_prng.c msws_prng.h)

add_executable(test_aes test_aes.c)
target_link_libraries(test_aes libcrypto)