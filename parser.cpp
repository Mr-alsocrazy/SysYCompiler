#pragma clang diagnostic push
#pragma ide diagnostic ignored "misc-no-recursion"
//
// Created by Administrator on 2022/9/22.
//

#include "parser.h"

vector<shared_ptr<string>> to_print;
bool can_return = false;
bool can_check_func_R = false;
using ident_block = shared_ptr<vector<shared_ptr<ident>>>;
using tid = table::identifier;
using tsy = table::sym;
using tet = table::error_type;

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

tsy parser::cur_token_sym() const {
    return cur_token->get_symcode();
}

void parser::judge_sym(tsy cur_sym) {
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
    bool main_end = false;
    while (cur_token_sym() == tsy::INTTK || cur_token_sym() == tsy::CONSTTK ||
           cur_token_sym() == tsy::VOIDTK) {
        if (cur_token_sym() == tsy::CONSTTK) {
            decl();
        } else if (cur_token_sym() == tsy::INTTK) {
            next_token();
            if (cur_token_sym() == tsy::IDENFR) {
                next_token();
                if (cur_token_sym() == tsy::LPARENT) {
                    prev_token();
                    prev_token();
                    func_def();
                } else {
                    prev_token();
                    prev_token();
                    decl();
                }
            } else if (cur_token_sym() == tsy::MAINTK) {
                prev_token();
                main_func_def();
                main_end = true;
            } else {
                throw exception();
            }
        } else if (cur_token_sym() == tsy::VOIDTK) {
            func_def();
        } else if (!main_end) {
            main_func_def();
        }
    }
    print_grm("CompUnit");
}

void parser::decl() {
    if (cur_token_sym() == tsy::CONSTTK) {
        const_decl();
    } else if (cur_token_sym() == tsy::INTTK) {
        var_decl();
    } else {
        throw exception();
    }
}

void parser::const_decl() {
    judge_sym(tsy::CONSTTK);
    if (cur_token_sym() == tsy::INTTK) {
        next_token();
        const_def();
    } else {
        throw exception();
    }
    int line_num = cur_token->get_linenum();
    while (cur_token_sym() == tsy::COMMA) {
        next_token();
        line_num = cur_token->get_linenum();
        const_def();
    }
    judge_err(tsy::SEMICN, line_num, tet::NO_SEMICN);
    print_grm("ConstDecl");
}

void parser::const_def() {
    judge_sym(tsy::IDENFR);
    int line_num = cur_token->get_linenum(), dimension = 0;
    ident idenfr(cur_token, true);
    while (cur_token_sym() == tsy::LBRACK) {
        dimension++;
        next_token();
        const_exp();
        judge_err(tsy::RBRACK, cur_token->get_linenum(), tet::NO_RBRACK);
    }
    idenfr.set_ident_type(ident::get_ident_type(dimension));
    if (ident_exist(idenfr)) {
        make_error(line_num, tet::IDENFR_REDEF);
    } else {
        add_ident(idenfr);
    }
    judge_sym(tsy::ASSIGN);
    const_init_val();
    print_grm("ConstDef");
}

void parser::const_init_val() {
    if (cur_token_sym() == tsy::LBRACE) {
        next_token();
        if (cur_token_sym() != tsy::RBRACE) {
            const_init_val();
            while (cur_token_sym() == tsy::COMMA) {
                next_token();
                const_init_val();
            }
        }
        judge_sym(tsy::RBRACE);
    } else {
        const_exp();
    }
    print_grm("ConstInitVal");
}

void parser::var_decl() {
    judge_sym(tsy::INTTK);
    var_def();
    int line_num = cur_token->get_linenum();
    while (cur_token_sym() == tsy::COMMA) {
        next_token();
        var_def();
        line_num = cur_token->get_linenum();
    }
    judge_err(tsy::SEMICN, line_num, tet::NO_SEMICN);
    print_grm("VarDecl");
}

void parser::var_def() {
    if (cur_token_sym() == tsy::IDENFR) {
        ident idenfr(cur_token, false);
        int line_num = cur_token->get_linenum(), dimension = 0;
        next_token();
        while (cur_token_sym() == tsy::LBRACK) {
            dimension++;
            next_token();
            const_exp();
            judge_err(tsy::RBRACK, cur_token->get_linenum(), tet::NO_RBRACK);
        }
        idenfr.set_ident_type(ident::get_ident_type(dimension));
        if (ident_exist(idenfr)) {
            make_error(line_num, tet::IDENFR_REDEF);
        } else {
            add_ident(idenfr);
        }
        if (cur_token_sym() == tsy::ASSIGN) {
            next_token();
            init_val();
        }
    } else {
        throw exception();
    }
    print_grm("VarDef");
}

void parser::init_val() {
    if (cur_token_sym() == tsy::LBRACE) {
        next_token();
        if (cur_token_sym() != tsy::RBRACE) {
            init_val();
            while (cur_token_sym() == tsy::COMMA) {
                next_token();
                init_val();
            }
        }
        judge_sym(tsy::RBRACE);
    } else {
        exp();
    }
    print_grm("InitVal");
}

void parser::main_func_def() {
    judge_sym(tsy::INTTK);
    judge_sym(tsy::MAINTK);
    int line_num = cur_token->get_linenum();
    judge_sym(tsy::LPARENT);
    judge_err(tsy::RPARENT, line_num, tet::NO_RPARENT);
    block(true, false, false);
    print_grm("MainFuncDef");
}

void parser::func_def() {
    while (cur_token_sym() == tsy::INTTK || cur_token_sym() == tsy::VOIDTK) {
        bool is_void = false;
        if (cur_token_sym() == tsy::VOIDTK) {
            is_void = true;
        }
        func_type();
        if (cur_token_sym() == tsy::MAINTK) {
            prev_token();
            break;
        }
        if (cur_token_sym() == tsy::IDENFR) {
            func func_idenfr(cur_token, is_void);
            if (func_exist(func_idenfr)) {
                make_error(cur_token->get_linenum(), tet::IDENFR_REDEF);
            } else {
                add_func(func_idenfr);
            }
            next_token();
        } else {
            throw exception();
        }
        if (cur_token_sym() == tsy::LPARENT) {
            bool param_exist = false, is_error = false;
            int line_num = cur_token->get_linenum();
            next_token();
            if (cur_token_sym() != tsy::RPARENT && cur_token_sym() != tsy::INTTK) {
                is_error = true;
                make_error(line_num, tet::NO_RPARENT);
            } else if (cur_token_sym() == tsy::RPARENT) {
                line_num = cur_token->get_linenum();
                param_exist = true;
                ident_block ib;
                ident_table.push_back(ib);
                func_F_params();
            }
            if (!is_error) {
                judge_err(tsy::RPARENT, line_num, tet::NO_RPARENT);
            }
            can_return = !is_void;
            block(!is_void, false, param_exist);
            can_return = false;
        } else {
            throw exception();
        }
        print_grm("FuncDef");
    }
}

void parser::func_F_params() {
    func_F_param();
    while (cur_token_sym() == tsy::COMMA) {
        next_token();
        func_F_param();
    }
    print_grm("FuncFParams");
}

void parser::func_F_param() {
    judge_sym(tsy::INTTK);
    if (cur_token_sym() == tsy::IDENFR) {
        int dimension = 0;
        ident func_idenfr(cur_token, false);
        int line_num = cur_token->get_linenum();
        next_token();
        if (cur_token_sym() == tsy::LBRACK) {
            next_token();
            judge_err(tsy::RBRACK, cur_token->get_linenum(), tet::NO_RBRACK);
            dimension++;
            while (cur_token_sym() == tsy::LBRACK) {
                dimension++;
                next_token();
                const_exp();
                judge_err(tsy::RBRACK, cur_token->get_linenum(), tet::NO_RBRACK);
            }
            func_idenfr.set_ident_type(ident::get_ident_type(dimension));
            if (ident_exist(func_idenfr)) {
                make_error(line_num, tet::IDENFR_REDEF);
            } else {
                add_ident(func_idenfr);
            }
            func_table.back()->add_param(ident::get_ident_type(dimension));
        }
    } else {
        throw exception();
    }
    if (cur_token_sym() == tsy::LBRACK) {
        next_token();
        judge_err(tsy::RBRACK, cur_token->get_linenum(), tet::NO_RBRACK);
        while (cur_token_sym() == tsy::LBRACK) {
            next_token();
            const_exp();
            judge_err(tsy::RBRACK, cur_token->get_linenum(), tet::NO_RBRACK);
        }
    }
    print_grm("FuncFParam");
}

void parser::func_R_params(const shared_ptr<func>& F, int i) {
    auto params = F->get_param_list();
    int para_cnt(0);
    int is_first(true), param_exist(false);
    auto it = params.begin();
    can_check_func_R = true;
    do {
        if(!is_first) {
            param_exist = true;
            next_token();
        }
        tid result = exp();
        if (result != tid::UNDEF) {
            para_cnt++;
            if (!params.empty() && *it != result && it != params.end()) {
                make_error(i, tet::PARA_TYPE_NOT_MATCH);
            }
        }
        if (it != params.end())
            it++;
        is_first = false;
    } while (cur_token_sym() != tsy::COMMA);
    if (!F && para_cnt != params.size()) {
        make_error(i, tet::PARA_NUM_NOT_MATCH);
    }
    can_check_func_R = false;
    print_grm("FuncRParams");
}

void parser::func_type() {
    if (cur_token_sym() == tsy::INTTK || cur_token_sym() == tsy::VOIDTK) {
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

tid parser::exp() {
    auto result = add_exp();
    print_grm("Exp");
    return result;
}

tid parser::add_exp() {
    auto result = mul_exp();
    print_grm("AddExp");
    while (cur_token_sym() == tsy::PLUS || cur_token_sym() == tsy::MINU) {
        next_token();
        mul_exp();
        print_grm("AddExp");
    }
    return result;
}

tid parser::mul_exp() {
    auto result = unary_exp();
    print_grm("MulExp");
    while (cur_token_sym() == tsy::MULT || cur_token_sym() == tsy::DIV ||
           cur_token_sym() == tsy::MOD) {
        next_token();
        unary_exp();
        print_grm("MulExp");
    }
    return result;
}

tid parser::unary_exp() {
    tid result = tid::UNDEF;
    if (cur_token_sym() == tsy::IDENFR) {
        auto keyf = find_func(cur_token->get_name());
        int line_num = cur_token->get_linenum();
        next_token();
        if (cur_token_sym() == tsy::LPARENT) {
            int ln_lpbr = cur_token->get_linenum();
            if (keyf == nullptr) {
                make_error(ln_lpbr, tet::IDENFR_UNDEF);
            } else if (!keyf->check_void()) {
                result = tid::INTEGER;
            } else {
                result = tid::VOID;
            }
            ln_lpbr = cur_token->get_linenum();
            next_token();
            if (cur_token_sym() != tsy::RPARENT) {
                func_R_params(keyf, line_num);
            } else if (keyf != nullptr && !keyf->get_param_list().empty()) {
                make_error(ln_lpbr, tet::PARA_NUM_NOT_MATCH);
            }
            judge_err(tsy::RPARENT, ln_lpbr, tet::NO_RPARENT);
        } else {
            prev_token();
            result = primary_exp();
        }
    } else if (cur_token_sym() == tsy::PLUS || cur_token_sym() == tsy::MINU || cur_token_sym() == tsy::NOT) {
        next_token();
        print_grm("UnaryOp");
        result = unary_exp();
    } else {
        result = primary_exp();
    }
    print_grm("UnaryExp");
    return result;
}

tid parser::primary_exp() {
    tid result = tid::UNDEF;
    if (cur_token_sym() == tsy::LPARENT) {
        int ln = cur_token->get_linenum();
        next_token();
        exp();
        judge_err(tsy::RPARENT, ln, tet::NO_RPARENT);
        result = tid::INTEGER;
    } else if (cur_token_sym() == tsy::IDENFR) {
        result = lval();
    } else if (cur_token_sym() == tsy::INTCON) {
        next_token();
        print_grm("Number");
        result = tid::INTEGER;
    } else {
        throw exception();
    }
    print_grm("PrimaryExp");
    return result;
}

tid parser::lval() {
    int dimension = 0;
    bool nid(false);
    tid result(tid::INTEGER);
    if (cur_token_sym() == table::sym::IDENFR) {
        ident id(ident(shared_ptr<Token>(cur_token), false));
        if (!ident_exist(id)) {
            make_error(cur_token->get_linenum(), tet::IDENFR_UNDEF);
        }
        result = find_ident(cur_token->get_name())->get_ident_type();
        next_token();
    }
    while (cur_token_sym() == tsy::LBRACK) {
        dimension++;
        next_token();
        exp();
        judge_err(tsy::RBRACK, cur_token->get_linenum(), tet::NO_RBRACK);
    }
    if (can_check_func_R) {
        if (result == tid::TWO_DIMENSION_ARRAY) {
            switch (dimension) {
                case 0:
                    result = tid::TWO_DIMENSION_ARRAY;
                    break;
                case 1:
                    result = tid::ONE_DIMENSION_ARRAY;
                    break;
                case 2:
                    result = tid::INTEGER;
                    break;
                default:
                    break;
            }
        } else if (result == tid::ONE_DIMENSION_ARRAY) {
            switch (dimension) {
                case 0:
                    result = tid::ONE_DIMENSION_ARRAY;
                    break;
                case 1:
                    result = tid::INTEGER;
                    break;
                default:
                    break;
            }
        } else if (result == tid::UNDEF && !nid) {
            switch (dimension) {
                case 0:
                    result = tid::INTEGER;
                    break;
                case 1:
                    result = tid::ONE_DIMENSION_ARRAY;
                    break;
                case 2:
                    result = tid::TWO_DIMENSION_ARRAY;
                    break;
                default:
                    break;
            }
        }
    }
    print_grm("LVal");
    return result;
}

void parser::block() {
    judge_sym(tsy::LBRACE);
    while (cur_token_sym() != tsy::RBRACE) {
        block_item();
    }
    judge_sym(tsy::RBRACE);
    print_grm("Block");
}

void parser::block(bool require_return, bool is_cycle, bool params_exist) {
    if (cur_token_sym() == tsy::LBRACE) {
        bool no_blocks(false), return_exist(false); // if there is nothin in the block
        if (!params_exist) {
            ident_table.push_back(make_shared<vector<shared_ptr<ident>>>());
        }
        next_token();
        if (cur_token_sym() == tsy::RBRACE) {
            no_blocks = true;
        } else {
            while (cur_token_sym() != tsy::RBRACE)
            {
                bool tmp = block_item(require_return, is_cycle);
                if (!return_exist)
                    return_exist = tmp;
            }
        }
        if (cur_token_sym() == tsy::RBRACE) {
            if (require_return && no_blocks) {
                make_error(cur_token->get_linenum(), tet::NO_RETURN);
            }
            if (require_return && !return_exist) {
                make_error(cur_token->get_linenum(), tet::NO_RETURN);
            }
            ident_table.pop_back();
            next_token();
        } else {
            throw exception();
        }
    }
    print_grm("Block");
}

void parser::block_item() {
    if (cur_token_sym() == tsy::INTTK || cur_token_sym() == tsy::CONSTTK) {
        decl();
    } else {
        stmt(false, false);
    }
}

bool parser::block_item(bool require_return, bool is_cycle) {
    bool return_exist(false);
    if (cur_token_sym() == tsy::INTTK || cur_token_sym() == tsy::CONSTTK) {
        decl();
        return false;
    } else {
        return stmt(require_return, is_cycle);
    }
}

bool parser::stmt(bool require_return, bool is_cycle) {
    int line_num = cur_token->get_linenum();
    if (cur_token_sym() == tsy::IFTK) {
        next_token();
        int ln_ifbr = cur_token->get_linenum();
        judge_sym(tsy::LPARENT);
        cond();
        judge_err(tsy::RPARENT, ln_ifbr, tet::NO_RPARENT);
        if (require_return) {
            can_return = true;
        }
        stmt(false, is_cycle);
        if (cur_token_sym() == tsy::ELSETK) {
            next_token();
            if (require_return)
                can_return = true;
            stmt(false, is_cycle);
        }
    } else if (cur_token_sym() == tsy::WHILETK) {
        next_token();
        int ln_whbr = cur_token->get_linenum();
        judge_sym(tsy::LPARENT);
        cond();
        judge_err(tsy::RPARENT, ln_whbr, tet::NO_RPARENT);
        can_return = require_return;
        stmt(false, true);
        can_return = false;
    } else if (cur_token_sym() == tsy::BREAKTK || cur_token_sym() == tsy::CONTINUETK) {
        if (!is_cycle) {
            make_error(cur_token->get_linenum(), tet::CONTINUE_BREAK_ERR);
        }
        int ln_cbbr = cur_token->get_linenum();
        next_token();
        judge_err(tsy::SEMICN, line_num, tet::NO_SEMICN);
    } else if (cur_token_sym() == tsy::RETURNTK) {
        bool return_exist(false);
        int ln_rtbr = cur_token->get_linenum();
        next_token();
        if (require_return || can_return) {
            return_exist = true;
            exp();
            judge_err(tsy::SEMICN, ln_rtbr, tet::NO_SEMICN);
        } else if (cur_token_sym() != tsy::SEMICN) {
            if (cur_token->get_linenum() - ln_rtbr != 1) {
                make_error(cur_token->get_linenum(), tet::RETURN_NOT_MATCH);
                exp();
            }
            judge_err(tsy::SEMICN, ln_rtbr, tet::NO_SEMICN);
        } else {
            judge_err(tsy::SEMICN, ln_rtbr, tet::NO_SEMICN);
        }
        return return_exist;
    } else if (cur_token_sym() == tsy::PRINTFTK) {
        int print_line_num = cur_token->get_linenum();
        next_token();
        judge_sym(tsy::LPARENT);
        int var_cnt = 0, d_cnt = 0;
        if (cur_token_sym() == tsy::STRCON) {
            std::string pr_str = cur_token->get_name();
            for (auto iter = pr_str.begin(); iter != pr_str.end(); iter++) {
                if (*iter == '%') {
                    iter++;
                    if (*iter == 'd') {
                        d_cnt++;
                    } else {
                        make_error(cur_token->get_linenum(), tet::SYMBOL_ILLICIT);
                        break;
                    }
                } else if (*iter == '\\') {
                    iter++;
                    if (*iter != 'n') {
                        make_error(cur_token->get_linenum(), tet::SYMBOL_ILLICIT);
                        break;
                    }
                } else if (!(*iter >= '(' && *iter <= '~' || *iter == 32 || *iter == 33 || *iter == '\"')) {
                    make_error(cur_token->get_linenum(), tet::SYMBOL_ILLICIT);
                    break;
                }
            }
            next_token();
        } else {
            throw exception();
        }
        while (cur_token_sym() == tsy::COMMA) {
            next_token();
            exp();
            var_cnt++;
        }
        if (var_cnt != d_cnt) {
            make_error(cur_token->get_linenum(), tet::PRINTF_PARA_NOT_MATCH);
        }
        judge_err(tsy::RPARENT, print_line_num, tet::NO_RPARENT);
        judge_err(tsy::SEMICN, line_num, tet::NO_SEMICN);
    } else if (cur_token_sym() == tsy::LBRACE) {
        if (require_return)
            can_return = true;
        block(false, is_cycle, false);
    } else if (cur_token_sym() == tsy::IDENFR) {
        next_token();
        if (cur_token_sym() != tsy::LPARENT) {
            prev_token();
            int tmp_tkps = token_pos;
            size_t p_len = to_print.size();
            lval();
            if (cur_token_sym() != tsy::ASSIGN) {
                token_pos = tmp_tkps;
                cur_token = token_vec.at(token_pos);
                size_t p_cur_len = to_print.size();
                for (size_t i = p_cur_len; i > p_len; --i) {
                    to_print.pop_back();
                }
                exp();
                judge_sym(tsy::SEMICN);
            } else {
                next_token();
                if (cur_token_sym() == tsy::GETINTTK) {
                    next_token();
                    judge_sym(tsy::LPARENT);
                    judge_sym(tsy::RPARENT);
                } else {
                    exp();
                }
                judge_sym(tsy::SEMICN);
            }
        } else {
            prev_token();
            exp();
            judge_sym(tsy::SEMICN);
        }
    } else if (cur_token_sym() == tsy::LPARENT || cur_token_sym() == tsy::INTTK ||
               cur_token_sym() == tsy::INTCON
               || cur_token_sym() == tsy::PLUS || cur_token_sym() == tsy::MINU ||
               cur_token_sym() == tsy::NOT) {
        exp();
        judge_err(tsy::SEMICN, line_num, tet::NO_SEMICN);
    } else {
        judge_err(tsy::SEMICN, line_num, tet::NO_SEMICN);
    }
    print_grm("Stmt");
    return false;
}

void parser::cond() {
    l_or_exp();
    print_grm("Cond");
}

void parser::l_or_exp() {
    l_and_exp();
    print_grm("LOrExp");
    while (cur_token_sym() == tsy::OR) {
        next_token();
        l_and_exp();
        print_grm("LOrExp");
    }
}

void parser::l_and_exp() {
    eq_exp();
    print_grm("LAndExp");
    while (cur_token_sym() == tsy::AND) {
        next_token();
        eq_exp();
        print_grm("LAndExp");
    }
}

void parser::eq_exp() {
    rel_exp();
    print_grm("EqExp");
    while (cur_token_sym() == tsy::EQL || cur_token_sym() == tsy::NEQ) {
        next_token();
        rel_exp();
        print_grm("EqExp");
    }
}

void parser::rel_exp() {
    add_exp();
    print_grm("RelExp");
    while (cur_token_sym() == tsy::GRE || cur_token_sym() == tsy::LSS ||
           cur_token_sym() == tsy::GEQ || cur_token_sym() == tsy::LEQ) {
        next_token();
        add_exp();
        print_grm("RelExp");
    }
}

void parser::make_error(int line_num, tet type) {
    error_vec.emplace_back(line_num, type);
    std::cout << line_num << ": " << table::error_table.find(type)->second << std::endl;
}

void parser::judge_err(tsy cur_sym, int line_num, tet err) {
    if (cur_token_sym() == cur_sym) {
        next_token();
    } else {
        make_error(line_num, err);
    }
}

bool parser::ident_exist(ident I) const {
    for (auto it_l1 = ident_table.rbegin(); it_l1 != ident_table.rend(); it_l1++) {
        for (auto & iptr : *(*it_l1)) {
            if (*iptr == I)
                return true;
        }
    }
    return false;
}

void parser::add_ident(const ident& I) {
    ident_table.back()->push_back(make_shared<ident>(I));
}

bool parser::func_exist(func &F) const {
    for (const auto & fptr : func_table) {
        if (*fptr == F)
            return true;
    }
    return false;
}

void parser::add_func(const func &F) {
    func_table.push_back(make_shared<func>(F));
}

shared_ptr<func> parser::find_func(const string &str) {
    for (auto& fptr : func_table) {
        if ((*fptr).get_func_name() == str) {
            return fptr;
        }
    }
    return nullptr;
}

shared_ptr<ident> parser::find_ident(const string &str) {
    for (auto it = ident_table.rbegin(); it != ident_table.rend(); it++) {
        for (auto& id : **it) {
            if (id->get_ident()->get_name() == str)
                return id;
        }
    }
    return nullptr;
}

#pragma clang diagnostic pop