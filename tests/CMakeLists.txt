add_executable(test_lexer_gen test_lexer_gen.cpp)
add_executable(test_lexer test_lexer.cpp)

target_link_libraries (test_lexer_gen LexerGen)
target_link_libraries (test_lexer Lexer LexerGen)

add_test(lexer_gen test_lexer_gen)
add_test(lexer test_lexer)