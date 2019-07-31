add_executable(test_dfa_gen test_dfa_gen.cpp)

target_link_libraries (test_dfa_gen LexerGen)

add_test(dfa_gen test_dfa_gen)