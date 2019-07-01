cmake_minimum_required(VERSION 2.8.9)
project (olang)

add_subdirectory (lexer_dfa_gen)
add_executable(olang main.cpp)
target_link_libraries (olang LexerGen)