//
// Created by Administrator on 2022/10/15.
//

#ifndef LEXICALANALYSIS_ERROR_H
#define LEXICALANALYSIS_ERROR_H

#include <iostream>
#include "tables.h"
#include <iostream>

class error {
private:
    int line_num;
    table::error_type type;
public:
    error(int _line_num, table::error_type _type) :
            line_num(_line_num), type(_type) {}

    string to_string() const;

    int get_line_num() const { return line_num; }
};


#endif //LEXICALANALYSIS_ERROR_H
