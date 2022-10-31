//
// Created by Administrator on 2022/10/23.
//

#ifndef LEXICALANALYSIS_FUNC_H
#define LEXICALANALYSIS_FUNC_H

#include "Token.h"
#include "tables.h"
#include <utility>
#include <vector>
#include <memory>

class func {
private:
    shared_ptr<Token> func_ident_ptr;
    vector<table::identifier> param_type;
    bool is_void;
public:
    func(shared_ptr<Token> _func_ident_ptr, bool _is_void) :
            func_ident_ptr(std::move(_func_ident_ptr)), is_void(_is_void) {}

    bool check_void() const { return is_void; }

    string get_func_name() const { return func_ident_ptr->get_name(); }

    void set_params(vector<table::identifier> &_param_type) { param_type = _param_type; }

    void add_param(table::identifier type);

    vector<table::identifier> get_param_list() const;

    bool operator==(func &f) const {
        return (f.func_ident_ptr->get_name() == this->func_ident_ptr->get_name());
    }
};


#endif //LEXICALANALYSIS_FUNC_H
