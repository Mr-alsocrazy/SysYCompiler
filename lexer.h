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
    lexer(ifstream _ifs, int _lpos) :
            ifs(std::move(_ifs)), lpos(_lpos), cpos(0), is_end(false) {
        ifs.open("testfile.txt");
    }

    void next_char();

    void roll_back();

    static pair<bool, table::sym> is_reserved(string &str);

    void get_sym();

    const ifstream& get_ifs() const { return ifs; }

    void print_tk();
};



#endif //LEXICALANALYSIS_LEXER_H
