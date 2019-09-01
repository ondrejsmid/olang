cmake_minimum_required(VERSION 2.8.9)
project (olang)

add_subdirectory (lexer)
add_subdirectory (lexer_gen)

add_executable(olang main.cpp)
target_link_libraries (olang Lexer LexerGen)

enable_testing()
add_subdirectory(tests)