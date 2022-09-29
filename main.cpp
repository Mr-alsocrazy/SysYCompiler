#include "lexer.h"
#include "parser.h"
#include <iostream>
#include <fstream>

using namespace std;

int main() {
    lexer lxr("testfile.txt");
    lxr.print_tk();
    parser psr();
    return 0;
}
