//
// Created by Administrator on 2022/10/23.
//

#include "func.h"

void func::add_param(table::identifier type) {
    this->param_type.push_back(type);
}

vector<table::identifier> func::get_param_list() const {
    return param_type;
}
