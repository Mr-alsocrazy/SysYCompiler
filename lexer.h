//
// Created by Administrator on 2022/9/12.
//

#ifndef LEXICALANALYSIS_LEXER_H
#define LEXICALANALYSIS_LEXER_H

#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include "tables.h"
#include "Token.h"

using namespace std;

vector<Token *> token_vec;

static bool in_comment;

class lexer {
private:
    string cur_line;
    char cur_char;
    int cpos;
    int lpos;
    int line_length;
    bool is_end;
public:
    lexer(string _cur_line, int _lpos) :
            cur_line(std::move(_cur_line)), lpos(_lpos), cpos(0), line_length(_cur_line.size()), is_end(false) {}

    void next_char();

    void roll_back();

    static pair<bool, table::sym> is_reserved(string &str);

    void get_sym();
};

void read_file();

#endif //LEXICALANALYSIS_LEXER_H
