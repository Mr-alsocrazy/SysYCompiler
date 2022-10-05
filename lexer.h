//
// Created by Administrator on 2022/9/12.
//

#ifndef LEXICALANALYSIS_LEXER_H
#define LEXICALANALYSIS_LEXER_H

#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <memory>
#include "tables.h"
#include "Token.h"

using namespace std;

extern vector<shared_ptr<Token>> token_vec;

static bool record_format;

class lexer {
private:
    ifstream ifs;
    char cur_char;
    string cur_line;
    int line_length;
    int l_id;
    int cpos;
    int lpos;
    bool is_end;
public:
    lexer(const string& filename) :
            l_id(0), lpos(0), cpos(0), is_end(false), line_length(0) {
        ifs.open(filename);
//        cout << ifs.is_open() << endl;
        next_char();
    }

    void next_char();

    void roll_back();

    static pair<bool, table::sym> is_reserved(string &str);

    void get_sym();

    void print_tk();

    void lexing();
};



#endif //LEXICALANALYSIS_LEXER_H
