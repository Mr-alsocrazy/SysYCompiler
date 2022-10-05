//
// Created by Administrator on 2022/9/22.
//

#include "parser.h"

vector<shared_ptr<string>> to_print;

void parser::parse() {
    comp_unit();
}

void parser::print_psr() {
    auto it = to_print.begin();
    std::ofstream ofs;
    ofs.open("output.txt");
    for (; it != to_print.end(); it++) {
        ofs << **it << endl;
    }
    ofs.close();
}

void parser::next_token() {
    if (token_pos < token_len - 1) {
        to_print.push_back(make_shared<string>(cur_token->token_str()));
//        std::cout << cur_token->token_str() << std::endl;
        cur_token = token_vec.at(++token_pos);
    } else if (token_pos == token_len - 1) {
        cur_token = token_vec.at(token_pos);
        to_print.push_back(make_shared<string>(cur_token->token_str()));
    }
}

void parser::prev_token() {
    cur_token = token_vec.at(--token_pos);
    while (to_print.back()->at(0) == '<')
        to_print.pop_back();
    to_print.pop_back();
}

table::sym parser::cur_token_sym() const {
    return cur_token->get_symcode();
}

void parser::judge_sym(table::sym cur_sym) {
    if (cur_token_sym() == cur_sym) {
        next_token();
    } else {
        throw exception();
    }
}

void parser::print_grm(const string &grm_name) {
    to_print.push_back(make_shared<string>('<' + grm_name + '>'));
//    std::cout << '<' + grm_name + '>' << std::endl;
}

void parser::comp_unit() {
    while (cur_token_sym() == table::sym::INTTK || cur_token_sym() == table::sym::CONSTTK ||
           cur_token_sym() == table::sym::VOIDTK) {
        if (cur_token_sym() == table::sym::CONSTTK) {
            decl();
        } else if (cur_token_sym() == table::sym::INTTK) {
            next_token();
            if (cur_token_sym() == table::sym::IDENFR) {
                next_token();
                if (cur_token_sym() == table::sym::LPARENT) {
                    prev_token();
                    prev_token();
                    func_def();
                } else {
                    prev_token();
                    prev_token();
                    decl();
                }
            } else if (cur_token_sym() == table::sym::MAINTK) {
                prev_token();
                main_func_def();
            } else {
                throw exception();
            }
        } else if (cur_token_sym() == table::sym::VOIDTK) {
            func_def();
        } else {
            throw exception();
        }
    }
    print_grm("CompUnit");
}

void parser::decl() {
    if (cur_token_sym() == table::sym::CONSTTK) {
        const_decl();
    } else if (cur_token_sym() == table::sym::INTTK) {
        var_decl();
    } else {
        throw exception();
    }
}

void parser::const_decl() {
    judge_sym(table::sym::CONSTTK);
    if (cur_token_sym() == table::sym::INTTK) {
        next_token();
        const_def();
    } else {
        throw exception();
    }
    while (cur_token_sym() == table::sym::COMMA) {
        next_token();
        const_def();
    }
    judge_sym(table::sym::SEMICN);
    print_grm("ConstDecl");
}

void parser::const_def() {
    judge_sym(table::sym::IDENFR);
    while (cur_token_sym() == table::sym::LBRACK) {
        next_token();
        const_exp();
        judge_sym(table::sym::RBRACK);
    }
    judge_sym(table::sym::ASSIGN);
    const_init_val();
    print_grm("ConstDef");
}

void parser::const_init_val() {
    if (cur_token_sym() == table::sym::LBRACE) {
        next_token();
        if (cur_token_sym() != table::sym::RBRACE) {
            const_init_val();
            while (cur_token_sym() == table::sym::COMMA) {
                next_token();
                const_init_val();
            }
        }
        judge_sym(table::sym::RBRACE);
    } else {
        const_exp();
    }
    print_grm("ConstInitVal");
}

void parser::var_decl() {
    judge_sym(table::sym::INTTK);
    var_def();
    while (cur_token_sym() == table::sym::COMMA) {
        next_token();
        var_def();
    }
    judge_sym(table::sym::SEMICN);
    print_grm("VarDecl");
}

void parser::var_def() {
    judge_sym(table::sym::IDENFR);
    while (cur_token_sym() == table::sym::LBRACK) {
        next_token();
        const_exp();
        judge_sym(table::sym::RBRACK);
    }
    if (cur_token_sym() == table::sym::ASSIGN) {
        next_token();
        init_val();
    }
    print_grm("VarDef");
}

void parser::init_val() {
    if (cur_token_sym() == table::sym::LBRACE) {
        next_token();
        if (cur_token_sym() != table::sym::RBRACE) {
            init_val();
            while (cur_token_sym() == table::sym::COMMA) {
                next_token();
                init_val();
            }
        }
        judge_sym(table::sym::RBRACE);
    } else {
        exp();
    }
    print_grm("InitVal");
}

void parser::main_func_def() {
    judge_sym(table::sym::INTTK);
    judge_sym(table::sym::MAINTK);
    judge_sym(table::sym::LPARENT);
    judge_sym(table::sym::RPARENT);
    block();
    print_grm("MainFuncDef");
}

void parser::func_def() {
    while (cur_token_sym() == table::sym::INTTK || cur_token_sym() == table::sym::VOIDTK) {
        func_type();
        if (cur_token_sym() == table::sym::MAINTK) {
            prev_token();
            break;
        }
        judge_sym(table::sym::IDENFR);
        judge_sym(table::sym::LPARENT);
        if (cur_token_sym() != table::sym::RPARENT) {
            func_F_params();
        }
        judge_sym(table::sym::RPARENT);
        block();
        print_grm("FuncDef");
    }
}

void parser::func_F_params() {
    func_F_param();
    while (cur_token_sym() == table::sym::COMMA) {
        next_token();
        func_F_param();
    }
    print_grm("FuncFParams");
}

void parser::func_F_param() {
    judge_sym(table::sym::INTTK);
    judge_sym(table::sym::IDENFR);
    if (cur_token_sym() == table::sym::LBRACK) {
        next_token();
        judge_sym(table::sym::RBRACK);
        while (cur_token_sym() == table::sym::LBRACK) {
            next_token();
            const_exp();
            judge_sym(table::sym::RBRACK);
        }
    }
    print_grm("FuncFParam");
}

void parser::func_R_params() {
    exp();
    while (cur_token_sym() == table::sym::COMMA) {
        next_token();
        exp();
    }
    print_grm("FuncRParams");
}

void parser::func_type() {
    if (cur_token_sym() == table::sym::INTTK || cur_token_sym() == table::sym::VOIDTK) {
        next_token();
    } else {
        throw exception();
    }
    print_grm("FuncType");
}

void parser::const_exp() {
    add_exp();
    print_grm("ConstExp");
}

void parser::exp() {
    add_exp();
    print_grm("Exp");
}

void parser::add_exp() {
    mul_exp();
    print_grm("AddExp");
    while (cur_token_sym() == table::sym::PLUS || cur_token_sym() == table::sym::MINU) {
        next_token();
        mul_exp();
        print_grm("AddExp");
    }
}

void parser::mul_exp() {
    unary_exp();
    print_grm("MulExp");
    while (cur_token_sym() == table::sym::MULT || cur_token_sym() == table::sym::DIV ||
           cur_token_sym() == table::sym::MOD) {
        next_token();
        unary_exp();
        print_grm("MulExp");
    }
}

void parser::unary_exp() {
    if (cur_token_sym() == table::sym::IDENFR) {
        next_token();
        if (cur_token_sym() == table::sym::LPARENT) {
            next_token();
            if (cur_token_sym() != table::sym::RPARENT) {
                func_R_params();
            }
            judge_sym(table::sym::RPARENT);
        } else {
            prev_token();
            primary_exp();
        }
    } else if (cur_token_sym() == table::sym::PLUS ||
               cur_token_sym() == table::sym::MINU ||
               cur_token_sym() == table::sym::NOT) {
        next_token();
        print_grm("UnaryOp");
        unary_exp();
    } else {
        primary_exp();
    }
    print_grm("UnaryExp");
}

void parser::primary_exp() {
    if (cur_token_sym() == table::sym::LPARENT) {
        next_token();
        exp();
        judge_sym(table::sym::RPARENT);
    } else if (cur_token_sym() == table::sym::IDENFR) {
        lval();
    } else if (cur_token_sym() == table::sym::INTCON) {
        next_token();
        print_grm("Number");
    } else {
        throw exception();
    }
    print_grm("PrimaryExp");
}

void parser::lval() {
    judge_sym(table::sym::IDENFR);
    while (cur_token_sym() == table::sym::LBRACK) {
        next_token();
        exp();
        judge_sym(table::sym::RBRACK);
    }
    print_grm("LVal");
}

void parser::block() {
    judge_sym(table::sym::LBRACE);
    while (cur_token_sym() != table::sym::RBRACE) {
        block_item();
    }
    judge_sym(table::sym::RBRACE);
    print_grm("Block");
}

void parser::block_item() {
    if (cur_token_sym() == table::sym::INTTK || cur_token_sym() == table::sym::CONSTTK) {
        decl();
    } else {
        stmt();
    }
}

void parser::stmt() {
    if (cur_token_sym() == table::sym::IFTK) {
        next_token();
        judge_sym(table::sym::LPARENT);
        cond();
        judge_sym(table::sym::RPARENT);
        stmt();
        if (cur_token_sym() == table::sym::ELSETK) {
            next_token();
            stmt();
        }
    } else if (cur_token_sym() == table::sym::WHILETK) {
        next_token();
        judge_sym(table::sym::LPARENT);
        cond();
        judge_sym(table::sym::RPARENT);
        stmt();
    } else if (cur_token_sym() == table::sym::BREAKTK || cur_token_sym() == table::sym::CONTINUETK) {
        next_token();
        judge_sym(table::sym::SEMICN);
    } else if (cur_token_sym() == table::sym::RETURNTK) {
        next_token();
        if (cur_token_sym() != table::sym::SEMICN) {
            exp();
        }
        judge_sym(table::sym::SEMICN);
    } else if (cur_token_sym() == table::sym::PRINTFTK) {
        next_token();
        judge_sym(table::sym::LPARENT);
        judge_sym(table::sym::STRCON);
        while (cur_token_sym() == table::sym::COMMA) {
            next_token();
            exp();
        }
        judge_sym(table::sym::RPARENT);
        judge_sym(table::sym::SEMICN);
    } else if (cur_token_sym() == table::sym::LBRACE) {
        block();
    } else if (cur_token_sym() == table::sym::IDENFR) {
        next_token();
        if (cur_token_sym() != table::sym::LPARENT) {
            prev_token();
            int tmp_tkps = token_pos;
            int p_len = to_print.size();
            lval();
            if (cur_token_sym() != table::sym::ASSIGN) {
                token_pos = tmp_tkps;
                cur_token = token_vec.at(token_pos);
                int p_cur_len = to_print.size();
                for (int i = p_cur_len; i > p_len; --i) {
                    to_print.pop_back();
                }
                exp();
                judge_sym(table::sym::SEMICN);
            } else {
                next_token();
                if (cur_token_sym() == table::sym::GETINTTK) {
                    next_token();
                    judge_sym(table::sym::LPARENT);
                    judge_sym(table::sym::RPARENT);
                } else {
                    exp();
                }
                judge_sym(table::sym::SEMICN);
            }
        } else {
            prev_token();
            exp();
            judge_sym(table::sym::SEMICN);
        }
    } else if (cur_token_sym() == table::sym::LPARENT || cur_token_sym() == table::sym::INTTK || cur_token_sym() == table::sym::INTCON
                || cur_token_sym() == table::sym::PLUS || cur_token_sym() == table::sym::MINU || cur_token_sym() == table::sym::NOT) {
        exp();
        judge_sym(table::sym::SEMICN);
    } else {
        judge_sym(table::sym::SEMICN);
    }
    print_grm("Stmt");
}

void parser::cond() {
    l_or_exp();
    print_grm("Cond");
}

void parser::l_or_exp() {
    l_and_exp();
    print_grm("LOrExp");
    while (cur_token_sym() == table::sym::OR) {
        next_token();
        l_and_exp();
        print_grm("LOrExp");
    }
}

void parser::l_and_exp() {
    eq_exp();
    print_grm("LAndExp");
    while (cur_token_sym() == table::sym::AND) {
        next_token();
        eq_exp();
        print_grm("LAndExp");
    }
}

void parser::eq_exp() {
    rel_exp();
    print_grm("EqExp");
    while (cur_token_sym() == table::sym::EQL || cur_token_sym() == table::sym::NEQ) {
        next_token();
        rel_exp();
        print_grm("EqExp");
    }
}

void parser::rel_exp() {
    add_exp();
    print_grm("RelExp");
    while (cur_token_sym() == table::sym::GRE || cur_token_sym() == table::sym::LSS ||
           cur_token_sym() == table::sym::GEQ || cur_token_sym() == table::sym::LEQ) {
        next_token();
        add_exp();
        print_grm("RelExp");
    }
}