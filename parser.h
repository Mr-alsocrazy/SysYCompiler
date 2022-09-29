//
// Created by Administrator on 2022/9/22.
//

#ifndef LEXICALANALYSIS_PARSER_H
#define LEXICALANALYSIS_PARSER_H
#include "Token.h"
#include "lexer.h"
#include <fstream>
#include <memory>
#include <string>

static vector<shared_ptr<string>> to_print;

class parser {
private:
    shared_ptr<Token> cur_token;
    int token_pos;
    int token_len;
public:
    parser() :token_pos(0), cur_token(nullptr) {
        cur_token = token_vec.at(token_pos);
        token_len = token_vec.size();
    }
    void parse();

    void next_token();

    void previous_token();

    table::sym cur_token_sym() const;

    void judge_sym(table::sym cur_sym);

    static void print_grm(const basic_string<char>& grm_name);

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

    void func_R_params();

    void const_exp();

    void exp();

    void block();

    void add_exp();

    void mul_exp();

    void unary_exp();

    void primary_exp();

    void lval();

    void block_item();

    void stmt();

    void cond();
};


#endif //LEXICALANALYSIS_PARSER_H
