cmake_minimum_required(VERSION 3.23)
project(LexicalAnalysis)

set(CMAKE_CXX_STANDARD 11)

add_executable(LexicalAnalysis main.cpp lexer.h lexer.cpp Token.cpp Token.h tables.h parser.cpp parser.h error.cpp error.h ident.cpp ident.h func.cpp func.h pcode.cpp pcode.h interpreter.cpp interpreter.h)
