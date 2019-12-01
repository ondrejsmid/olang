add_library(Parser parser.cpp)
target_link_libraries (Parser Lexer LexerGen)