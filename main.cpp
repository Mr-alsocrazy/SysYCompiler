#include "lexer.h"
#include <iostream>
#include <fstream>

using namespace std;

int main() {
    lexer lxr("../testfile.txt");
    lxr.print_tk();
    return 0;
}
