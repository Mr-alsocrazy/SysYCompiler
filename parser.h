//
// Created by Administrator on 2022/9/22.
//

#ifndef LEXICALANALYSIS_PARSER_H
#define LEXICALANALYSIS_PARSER_H

#include "Token.h"
#include "lexer.h"
#include "error.h"
#include "ident.h"
#include "func.h"
#include <fstream>
#include <memory>
#include <string>
#include <vector>
#include <algorithm>

extern vector<shared_ptr<string>> to_print;
extern bool can_return;
extern bool can_check_func_R;

class parser {
private:
    shared_ptr<Token> cur_token;
    std::vector<error> error_vec;
    int token_pos;
    int token_len;
    vector<shared_ptr<vector<shared_ptr<ident>>>> ident_table;
    vector<shared_ptr<func>> func_table;
public:
    parser() : token_pos(0), cur_token(nullptr) {
        cur_token = token_vec.at(token_pos);
        token_len = token_vec.size();
        ident_table.push_back(make_shared<vector<shared_ptr<ident>>>());
    }

    void parse();

    void next_token();

    void prev_token();

    table::sym cur_token_sym() const;

    void judge_sym(table::sym cur_sym);

    void judge_err(table::sym cur_sym, int line_num, table::error_type err);

    static void print_grm(const basic_string<char> &grm_name);

    void comp_unit();

    void decl();

    void const_decl();

    void const_def();

    void const_init_val();

    void var_decl();

    void var_def();

    void init_val();

    void main_func_def();

    void func_def();

    void func_type();

    void func_F_params();

    void func_F_param();

    void func_R_params(const shared_ptr<func>& F, int i);

    void const_exp();

    table::identifier exp();

    void block();

    void block(bool require_return, bool is_cycle, bool params_exist);

    table::identifier add_exp();

    table::identifier mul_exp();

    table::identifier unary_exp();

    table::identifier primary_exp();

    table::identifier lval();

    void block_item();

    bool block_item(bool require_return, bool is_cycle);

    bool stmt(bool require_return, bool is_cycle);

    void cond();

    void l_or_exp();

    void l_and_exp();

    void eq_exp();

    void rel_exp();

    static void print_psr();

    void make_error(int line_num, table::error_type type);

    bool ident_exist(ident I) const;

    void add_ident(const ident &I);

    bool func_exist(func &F) const;

    void add_func(const func &F);

    shared_ptr<func> find_func(const string& str);

    shared_ptr<ident> find_ident(const string& str);
};


#endif //LEXICALANALYSIS_PARSER_H
