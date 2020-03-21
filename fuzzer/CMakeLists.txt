add_executable(fuzz-parser FuzzParser.cpp)
target_link_libraries(fuzz-parser calc)

add_executable(fuzz-lexer FuzzLexer.cpp)
target_link_libraries(fuzz-lexer calc)