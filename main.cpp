#include "lexer.h"
#include "parser.h"
#include "interpreter.h"
#include <iostream>
#include <fstream>

using namespace std;

int main() {
    lexer lxr("testfile.txt");
    lxr.lexing();
//    lxr.print_tk();

    parser psr;
    psr.parse();
//    psr.print_err();
    parser::print_psr();
    parser::print_pcode();
    interpreter itp(main_idx);
    itp.interpret();
    itp.print_result();
    return 0;
}
