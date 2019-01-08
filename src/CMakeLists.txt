add_executable(test_aes test/test_aes.c)
target_link_libraries(test_aes LINK_PUBLIC crypto)