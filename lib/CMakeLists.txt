add_library(crypto SHARED aes.c msws_prng.c lcg_prng.c lcg_prng.h keccak.c keccak.h)
target_include_directories(crypto PUBLIC $(CMAKE_CURRENT_SOURCE_DIR))
install(TARGETS crypto DESTINATION lib)
install(FILES aes.h msws_prng.h lcg_prng.h DESTINATION include)