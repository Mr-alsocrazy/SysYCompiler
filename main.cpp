#include "lexer.h"
#include "parser.h"
#include <iostream>
#include <fstream>

using namespace std;

int main() {
    lexer lxr("testfile.txt");
    lxr.lexing();
//    lxr.print_tk();

    parser psr;
    psr.parse();
    parser::print_psr();
    return 0;
}
