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
    bool nonToken = false;
    Token(table::sym _symcode, int lime_num) : symcode(_symcode), line(lime_num) {
        nonToken = true;
    }

    Token(table::sym _symcode, string _name, int lime_num) :
            symcode(_symcode), name(std::move(_name)), line(lime_num) {}

    Token(table::sym _symcode, int _value, int lime_num) :
            symcode(_symcode), value(_value), line(lime_num) {
        nonToken = true;
    }

    Token() : value(0) {
        nonToken = true;
    }

    const string &get_name() const {
        return name;
    }

    const table::sym &get_symcode() const {
        return symcode;
    }

    const int &get_linenum() const {
        return line;
    }

    const int &get_value() const {
        return value;
    }

    void set_value(int instant) {
        value = instant;
    }

    void set_type(table::sym s) {
        symcode = s;
    }

    string token_str() const;
};


#endif //LEXICALANALYSIS_TOKEN_H
