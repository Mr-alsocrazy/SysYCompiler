//
// Created by Administrator on 2022/9/12.
//

#ifndef LEXICALANALYSIS_TOKEN_H
#define LEXICALANALYSIS_TOKEN_H

#include "tables.h"
#include <string>
#include <vector>

using namespace std;

class Token {
private:
    table::sym symcode;
    string name;
    int value{};
    int line;
public:
    Token(table::sym _symcode, int lime_num) : symcode(_symcode), name(""), line(lime_num) {}

    Token(table::sym _symcode, string _name, int lime_num) :
            symcode(_symcode), name(std::move(_name)), line(lime_num) {}

    Token(table::sym _symcode, int _value, int lime_num) :
            symcode(_symcode), value(_value), line(lime_num) {}

    const string &get_name() const {
        return name;
    }

    const table::sym &get_symcode() const {
        return symcode;
    }
};


#endif //LEXICALANALYSIS_TOKEN_H
