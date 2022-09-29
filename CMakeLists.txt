cmake_minimum_required(VERSION 3.23)
project(LexicalAnalysis)

set(CMAKE_CXX_STANDARD 11)

add_executable(LexicalAnalysis main.cpp lexer.h lexer.cpp Token.cpp Token.h tables.h parser.cpp parser.h exception.h)
