//
// Created by Administrator on 2022/10/15.
//

#include "error.h"

string error::to_string() const {
    return std::to_string(line_num) + " " + table::error_table.find(type)->second;
}
