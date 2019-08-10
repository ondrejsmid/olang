add_executable(test_dfa_gen test_dfa_gen.cpp)
add_executable(test_lexer test_lexer.cpp)

target_link_libraries (test_dfa_gen Dfa Lexer LexerDfaGen)
target_link_libraries (test_lexer Dfa Lexer LexerDfaGen)

add_test(dfa_gen test_dfa_gen)
add_test(lexer test_lexer)