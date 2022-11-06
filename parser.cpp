#pragma clang diagnostic push
#pragma ide diagnostic ignored "misc-no-recursion"
//
// Created by Administrator on 2022/9/22.
//

#include "parser.h"
//#define PSR_DEBUG

vector<shared_ptr<string>> to_print;
using ident_block = vector<shared_ptr<ident>>;
using tid = table::identifier;
using tsy = table::sym;
using tet = table::error_type;
using tp = table::pcode;
bool can_return = false; //是否需要返回值
bool can_check_func_R = false; //是否进行函数实参检查
bool is_func_block = false; //是否是函数定义产生的Block
bool is_void_func = false;
bool in_main = false;
int block_layer = 0;
bool attain_v_return = false;
bool over_exp = false;
int lval_in_exp = 0;
int dimension_decl = 0;
bool func_start = false;
size_t main_idx = 0;
std::vector<std::shared_ptr<ident>> func_params;
std::vector<shared_ptr<pcode>> code_vec;

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
#ifdef PSR_DEBUG
        std::cout << cur_token->token_str() << std::endl;
#endif
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
#ifdef PSR_DEBUG
    std::cout << '<' + grm_name + '>' << std::endl;
#endif
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
    int line_num = cur_token->get_linenum();
    if (cur_token_sym() == tsy::INTTK) {
        next_token();
        const_def();
    } else {
        throw exception();
    }
    while (cur_token_sym() == tsy::COMMA) {
        next_token();
        line_num = cur_token->get_linenum();
        const_def();
    }
    judge_err(tsy::SEMICN, line_num, tet::NO_SEMICN);
    print_grm("ConstDecl");
}

void parser::const_def() {
    if (cur_token_sym() == tsy::IDENFR) {
        int line_num = cur_token->get_linenum();
        ident idenfr(cur_token, true);
        add_pcode(tp::NEW, std::make_shared<ident>(idenfr));
        next_token();
        size_t idx(code_vec.size() - 1);
        while (cur_token_sym() == tsy::LBRACK) {
            dimension_decl++;
            next_token();
            const_exp();
            judge_err(tsy::RBRACK, cur_token->get_linenum(), tet::NO_RBRACK);
            if (dimension_decl == 1) {
                code_vec.at(idx)->set_ident_tpye(tid::ONE_DIMENSION_ARRAY);
                add_pcode(tp::BOUND1, make_shared<ident>(idenfr));
            } else {
                code_vec.at(idx)->set_ident_tpye(tid::TWO_DIMENSION_ARRAY);
                add_pcode(tp::BOUND2, make_shared<ident>(idenfr));
            }
        }
        idenfr.set_ident_type(ident::get_ident_type(dimension_decl));
        if (ident_exist(idenfr)) {
            make_error(line_num, tet::IDENFR_REDEF);
        } else {
            add_ident(idenfr);
        }
        judge_sym(tsy::ASSIGN);
        add_pcode(tp::LOAD, make_shared<ident>(idenfr));
        const_init_val();
        add_pcode(tp::STORIN);
        print_grm("ConstDef");
        dimension_decl = 0;
    } else {
        throw exception();
    }
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
        if (dimension_decl > 0)
            add_pcode(tp::ELEM);
    }
    print_grm("ConstInitVal");
}

void parser::var_decl() {
    judge_sym(tsy::INTTK);
    int line_num = cur_token->get_linenum();
    var_def();
    while (cur_token_sym() == tsy::COMMA) {
        next_token();
        line_num = cur_token->get_linenum();
        var_def();
    }
    judge_err(tsy::SEMICN, line_num, tet::NO_SEMICN);
    print_grm("VarDecl");
}

void parser::var_def() {
    if (cur_token_sym() == tsy::IDENFR) {
        ident idenfr(cur_token, false);
        add_pcode(tp::NEW, make_shared<ident>(idenfr));
        int line_num = cur_token->get_linenum();
        next_token();
        size_t idx(code_vec.size() - 1);
        while (cur_token_sym() == tsy::LBRACK) {
            dimension_decl++;
            next_token();
            const_exp();
            judge_err(tsy::RBRACK, cur_token->get_linenum(), tet::NO_RBRACK);
            if (dimension_decl == 1) {
                code_vec.at(idx)->set_ident_tpye(tid::ONE_DIMENSION_ARRAY);
                add_pcode(tp::BOUND1, make_shared<ident>(idenfr));
            } else {
                code_vec.at(idx)->set_ident_tpye(tid::TWO_DIMENSION_ARRAY);
                add_pcode(tp::BOUND2, make_shared<ident>(idenfr));
            }
        }
        idenfr.set_ident_type(ident::get_ident_type(dimension_decl));
        if (ident_exist(idenfr)) {
            make_error(line_num, tet::IDENFR_REDEF);
        } else {
            add_ident(idenfr);
        }
        if (cur_token_sym() == tsy::ASSIGN) {
            next_token();
            init_val();
            add_pcode(tp::STORIN);
        } else {
            add_pcode(tp::STO, make_shared<ident>(idenfr));
        }
        dimension_decl = 0;
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
        if (dimension_decl > 1)
            add_pcode(tp::ELEM);
    }
    print_grm("InitVal");
}

void parser::main_func_def() {
    in_main = true;
    judge_sym(tsy::INTTK);
    judge_sym(tsy::MAINTK);
    int line_num = cur_token->get_linenum();
    judge_sym(tsy::LPARENT);
    judge_err(tsy::RPARENT, line_num, tet::NO_RPARENT);
    attain_v_return = false;
    add_pcode(tp::LABEL, "main");
    main_idx = code_vec.size() - 1;
    block(true, false, false);
    print_grm("MainFuncDef");
}

void parser::func_def() {
    if (!func_start) {
        func_start = true;
        add_pcode(tp::FUNCDEF);
    }
    while (cur_token_sym() == tsy::INTTK || cur_token_sym() == tsy::VOIDTK) {
        attain_v_return = false;
        is_void_func = false;
        if (cur_token_sym() == tsy::VOIDTK)
            is_void_func = true;
        func_type();
        if (cur_token_sym() == tsy::MAINTK) {
            prev_token();
            break;
        }
        func func_idenfr(cur_token, is_void_func);
        if (cur_token_sym() == tsy::IDENFR) {
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
            add_pcode(func_idenfr.get_func_name());
            add_pcode(tp::INB);
            is_func_block = true;
            if (cur_token_sym() != tsy::RPARENT && cur_token_sym() != tsy::INTTK) {
                is_error = true;
                make_error(line_num, tet::NO_RPARENT);
            } else if (cur_token_sym() != tsy::RPARENT) {
                line_num = cur_token->get_linenum();
                param_exist = true;
                ident_block ib;
                ident_table.push_back(ib);
                func_F_params();
            }
            if (!is_error) {
                judge_err(tsy::RPARENT, line_num, tet::NO_RPARENT);
            }
            can_return = !is_void_func;
            in_main = false;
            attain_v_return = false;
            block(!is_void_func, false, param_exist);
            if (is_void_func && !attain_v_return && !in_main) {
                add_pcode(tp::RE, "void");
            }
            attain_v_return = true;
            in_main = true;
            can_return = false;
            is_void_func = false;
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
//    for (auto & fp : func_table.back()->get_param_list()) {
//        add_pcode(tp::STO, std::make_shared<ident>(fp));
//    }
    for (auto fp = func_params.rbegin(); fp != func_params.rend(); fp++) {
        add_pcode(tp::STO, *fp);
    }
    add_pcode(tp::PARAMEND);
    func_params.clear();
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
                over_exp = true;
                const_exp();
                over_exp = false;
                judge_err(tsy::RBRACK, cur_token->get_linenum(), tet::NO_RBRACK);
            }
        }
        func_idenfr.set_ident_type(ident::get_ident_type(dimension));
        if (ident_exist(func_idenfr)) {
            make_error(line_num, tet::IDENFR_REDEF);
        } else {
            add_ident(func_idenfr);
        }
        func_table.back()->add_param(ident::get_ident_type(dimension));
        func_params.push_back(std::make_shared<ident>(func_idenfr));
    } else {
        throw exception();
    }
    print_grm("FuncFParam");
}

void parser::func_R_params(const shared_ptr<func> &F, int line_num) {
    auto params = F->get_param_list();
    int para_cnt(0);
    int is_first(true), param_exist(false);
    auto it = params.begin();
    can_check_func_R = true;
    do {
        if (!is_first) {
            param_exist = true;
            next_token();
        }
        add_pcode(tp::FRP);
        int tmp = token_pos;
        size_t p_len = to_print.size();
        size_t e_len = error_vec.size();
        size_t c_len = code_vec.size();
        tid result = exp();
        token_pos = tmp;
        cur_token = token_vec.at(token_pos);
        size_t p_cur_len = to_print.size();
        size_t e_cur_len = error_vec.size();
        size_t c_cur_len = code_vec.size();
        for (size_t i = p_cur_len; i > p_len; i--)
            to_print.pop_back();
        for (size_t i = e_cur_len; i > e_len; i--)
            error_vec.pop_back();
        for (size_t i = c_cur_len; i > c_len; i--)
            code_vec.pop_back();
        add_pcode(tp::TYPE, make_shared<ident>(result));
        exp();
        if (result != tid::UNDEF) {
            para_cnt++;
            if (!params.empty() && *it != result && it != params.end()) {
                make_error(line_num, tet::PARA_TYPE_NOT_MATCH);
            }
        }
        if (it != params.end())
            it++;
        is_first = false;
    } while (cur_token_sym() == tsy::COMMA);
    if (F && para_cnt != params.size()) {
        make_error(line_num, tet::PARA_NUM_NOT_MATCH);
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
        auto type = cur_token_sym();
        next_token();
        auto r = mul_exp();
        if (can_check_func_R) {
            if (result == tid::UNDEF || r == tid::UNDEF)
                result = tid::UNDEF;
            else if (result == tid::VOID || r == tid::VOID)
                result = tid::VOID;
            else if (result == tid::INTEGER || r == tid::INTEGER)
                result = tid::INTEGER;
            else if (result == tid::ONE_DIMENSION_ARRAY || r == tid::ONE_DIMENSION_ARRAY)
                result = tid::ONE_DIMENSION_ARRAY;
            else if (result == tid::TWO_DIMENSION_ARRAY || r == tid::TWO_DIMENSION_ARRAY)
                result = tid::TWO_DIMENSION_ARRAY;
        }
        print_grm("AddExp");
        if (type == tsy::PLUS)
            add_pcode(tp::ADD);
        else
            add_pcode(tp::SUB);
    }
    return result;
}

tid parser::mul_exp() {
    auto result = unary_exp();
    print_grm("MulExp");
    while (cur_token_sym() == tsy::MULT || cur_token_sym() == tsy::DIV ||
           cur_token_sym() == tsy::MOD) {
        auto type = cur_token_sym();
        next_token();
        unary_exp();
        print_grm("MulExp");
        switch (type) {
            case tsy::MULT:
                add_pcode(tp::MULT);
                break;
            case tsy::DIV:
                add_pcode(tp::DIV);
                break;
            case tsy::MOD:
                add_pcode(tp::MOD);
            default:
                break;
        }
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
            if (cur_token_sym() != tsy::RPARENT && cur_token_sym() != tsy::SEMICN) {
                func_R_params(keyf, line_num);
            } else if (keyf != nullptr && !keyf->get_param_list().empty()) {
                make_error(ln_lpbr, tet::PARA_NUM_NOT_MATCH);
            }
            judge_err(tsy::RPARENT, ln_lpbr, tet::NO_RPARENT);
            add_pcode(tp::J, keyf->get_func_name());
        } else {
            prev_token();
            result = primary_exp();
        }
    } else if (cur_token_sym() == tsy::PLUS || cur_token_sym() == tsy::MINU || cur_token_sym() == tsy::NOT) {
        auto type = cur_token_sym();
        next_token();
        print_grm("UnaryOp");
        result = unary_exp();
        switch (type) {
            case tsy::MINU:
                add_pcode(tp::NEG);
                break;
            case tsy::NOT:
                add_pcode(tp::NOT);
                break;
            case tsy::PLUS:
                add_pcode(tp::POS);
                break;
            default:
                break;
        }
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
        lval_in_exp++;
        result = lval();
        lval_in_exp--;
    } else if (cur_token_sym() == tsy::INTCON) {
        add_pcode(tp::LOADI, cur_token->get_value());
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
        } else {
            result = find_ident(cur_token->get_name())->get_ident_type();
        }
        next_token();
        add_pcode(tp::LOAD, std::make_shared<ident>(id));
    }
    while (cur_token_sym() == tsy::LBRACK) {
        dimension++;
        next_token();
        exp();
        judge_err(tsy::RBRACK, cur_token->get_linenum(), tet::NO_RBRACK);
        if (lval_in_exp == 0) {
            if (dimension == 1)
                add_pcode(tp::SETD1);
            else
                add_pcode(tp::SETD2);
        } else {
            if (dimension == 1)
                add_pcode(tp::SETD1R);
            else
                add_pcode(tp::SETD2R);
        }
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
    block_layer++;
    if (cur_token_sym() == tsy::LBRACE) {
        bool no_blocks(false), return_exist(false); // if there is nothin in the block
        if (!is_func_block) {
            add_pcode(tp::INB);
        } else {
            is_func_block = false;
        }
        if (!params_exist) {
            ident_table.emplace_back();
        }
        next_token();
        if (cur_token_sym() == tsy::RBRACE) {
            no_blocks = true;
        } else {
            while (cur_token_sym() != tsy::RBRACE) {
                bool tmp = block_item(require_return, is_cycle);
                if (!return_exist)
                    return_exist = tmp;
            }
        }
        int line_num = cur_token->get_linenum();
        if (cur_token_sym() == tsy::RBRACE) {
            if (require_return && no_blocks) {
                make_error(line_num, tet::NO_RETURN);
            }
            if (require_return && !return_exist) {
                make_error(line_num, tet::NO_RETURN);
            }
            ident_table.pop_back();
            next_token();
            if (block_layer > 1)
                add_pcode(tp::OUTB);
        } else {
            throw exception();
        }
    }
    block_layer--;
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
        if (!in_main && !is_void_func) {
            add_pcode(tp::RE);
            attain_v_return = true;
        } else if (!in_main && is_void_func) {
            add_pcode(tp::RE, "VOID");
            if (block_layer == 1) {
                attain_v_return = true;
            }
        } else {
            add_pcode(tp::END);
        }
        print_grm("Stmt");
        return return_exist;
    } else if (cur_token_sym() == tsy::PRINTFTK) {
        int print_line_num = cur_token->get_linenum();
        next_token();
        judge_sym(tsy::LPARENT);
        int var_cnt = 0, d_cnt = 0;
        vector<pcode> pcode_vec_prbr;
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
                    pcode_vec_prbr.emplace_back(tp::PRINT, "%");
                } else if (*iter == '\\') {
                    iter++;
                    if (*iter != 'n') {
                        make_error(cur_token->get_linenum(), tet::SYMBOL_ILLICIT);
                        break;
                    }
                    pcode_vec_prbr.emplace_back(tp::PRINT, "\\n");
                } else if (!(*iter >= '(' && *iter <= '~' || *iter == 32 || *iter == 33 || *iter == '\"')) {
                    make_error(cur_token->get_linenum(), tet::SYMBOL_ILLICIT);
                    break;
                } else if (*iter != '\"') {
                    pcode_vec_prbr.emplace_back(tp::PRINT, std::string(1, *iter));
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
        int print_cnt(var_cnt);
        for (auto &pc: pcode_vec_prbr) {
            if (pc.get_label() == "%") {
                add_pcode(tp::PRINT, print_cnt);
                --print_cnt;
            } else {
                add_pcode(tp::PRINT, pc.get_label());
            }
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
        ident assign = ident(shared_ptr<Token>(cur_token), false);
        next_token();
        if (cur_token_sym() != tsy::LPARENT) {
            prev_token();
            int tmp_tkps = token_pos;
            size_t p_len = to_print.size();
            size_t c_len = code_vec.size();
            lval();
            if (cur_token_sym() != tsy::ASSIGN) {
                token_pos = tmp_tkps;
                cur_token = token_vec.at(token_pos);
                size_t p_cur_len = to_print.size();
                size_t c_cur_len = code_vec.size();
                for (size_t i = p_cur_len; i > p_len; --i) {
                    to_print.pop_back();
                }
                for (size_t i = c_cur_len; i > c_len; --i) {
                    code_vec.pop_back();
                }
                int ln_asbr = cur_token->get_linenum();
                over_exp = true;
                exp();
                over_exp = false;
                judge_err(tsy::SEMICN, ln_asbr, tet::NO_SEMICN);
            } else {
                auto item = find_ident(assign.get_ident()->get_name());
                if (item && item->get_is_const()) {
                    make_error(cur_token->get_linenum(), tet::CONST_MODIFY);
                } else if (!item) {
                    make_error(cur_token->get_linenum(), tet::IDENFR_UNDEF);
                }
                next_token();
                if (cur_token_sym() == tsy::GETINTTK) {
                    next_token();
                    add_pcode(tp::GETINT);
                    int ln_gibr = cur_token->get_linenum();
                    judge_sym(tsy::LPARENT);
                    judge_err(tsy::RPARENT, ln_gibr, tet::NO_RPARENT);
                } else {
                    exp();
                }
                add_pcode(tp::STORIN);
                judge_err(tsy::SEMICN, line_num, tet::NO_SEMICN);
            }
        } else {
            prev_token();
            exp();
            judge_err(tsy::SEMICN, line_num, tet::NO_SEMICN);
        }
    } else if (cur_token_sym() == tsy::LPARENT || cur_token_sym() == tsy::INTTK ||
               cur_token_sym() == tsy::INTCON
               || cur_token_sym() == tsy::PLUS || cur_token_sym() == tsy::MINU ||
               cur_token_sym() == tsy::NOT) {
        over_exp = true;
        exp();
        over_exp = false;
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
    bool or_exist(false);
    l_and_exp();
    add_pcode(tp::ORUNIT);
    print_grm("LOrExp");
    while (cur_token_sym() == tsy::OR) {
        or_exist = true;
        next_token();
        l_and_exp();
        add_pcode(tp::ORUNIT);
        print_grm("LOrExp");
        add_pcode(tp::OR);
    }
}

void parser::l_and_exp() {
    bool and_exist(false);
    eq_exp();
    add_pcode(tp::ANDUNIT);
    print_grm("LAndExp");
    while (cur_token_sym() == tsy::AND) {
        and_exist = true;
        next_token();
        eq_exp();
        add_pcode(tp::ANDUNIT);
        print_grm("LAndExp");
        add_pcode(tp::AND);
    }
}

void parser::eq_exp() {
    rel_exp();
    print_grm("EqExp");
    while (cur_token_sym() == tsy::EQL or cur_token_sym() == tsy::NEQ) {
        tsy ans = cur_token_sym();
        next_token();
        rel_exp();
        print_grm("EqExp");
        if (ans == tsy::EQL)
            add_pcode(tp::EQ);
        else
            add_pcode(tp::NEQ);
    }
}

void parser::rel_exp() {
    add_exp();
    print_grm("RelExp");
    while (cur_token_sym() == tsy::GRE || cur_token_sym() == tsy::LSS ||
           cur_token_sym() == tsy::GEQ || cur_token_sym() == tsy::LEQ) {
        tsy ans = cur_token_sym();
        next_token();
        add_exp();
        print_grm("RelExp");
        switch (ans) {
            case tsy::GRE:
                add_pcode(tp::GT);
                break;
            case tsy::LSS:
                add_pcode(tp::LES);
                break;
            case tsy::GEQ:
                add_pcode(tp::GEQ);
                break;
            case tsy::LEQ:
                add_pcode(tp::LEQ);
                break;
            default:
                break;
        }
    }
}

void parser::make_error(int line_num, tet type) {
    if (!error_vec.empty() && line_num == error_vec.back().get_line_num()) {
        error_vec.pop_back();
    }
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

bool parser::ident_exist(const ident &I) const {
    for (auto it_l1 = ident_table.rbegin(); it_l1 != ident_table.rend(); it_l1++) {
        for (auto &iptr: *it_l1) {
            if (*iptr == I)
                return true;
        }
    }
    return false;
}

void parser::add_ident(const ident &I) {
    ident_table.back().push_back(make_shared<ident>(I));
}

bool parser::func_exist(func &F) const {
    for (const auto &fptr: func_table) {
        if (*fptr == F)
            return true;
    }
    return false;
}

void parser::add_func(const func &F) {
    func_table.push_back(make_shared<func>(F));
}

shared_ptr<func> parser::find_func(const string &str) {
    for (auto &fptr: func_table) {
        if ((*fptr).get_func_name() == str) {
            return fptr;
        }
    }
    return nullptr;
}

shared_ptr<ident> parser::find_ident(const string &str) {
    for (auto it = ident_table.rbegin(); it != ident_table.rend(); it++) {
        for (auto &id: *it) {
            if (id->get_ident()->get_name() == str)
                return id;
        }
    }
    return nullptr;
}

void parser::print_err() {
    ofstream ofs;
    ofs.open("error.txt");
    for (auto &err: error_vec) {
        ofs << err.to_string() << endl;
    }
}

void parser::add_pcode(table::pcode code, const std::string &label) {
    if (!over_exp)
        code_vec.push_back(std::make_shared<pcode>(code, label));
}

void parser::add_pcode(table::pcode code) {
    if (!over_exp)
        code_vec.push_back(std::make_shared<pcode>(code));
}

void parser::add_pcode(table::pcode code, int instant) {
    if (!over_exp)
        code_vec.push_back(std::make_shared<pcode>(code, instant));
}

void parser::add_pcode(table::pcode code, const std::shared_ptr<ident> &id) {
    if (!over_exp)
        code_vec.push_back(std::make_shared<pcode>(code, id));
}

void parser::add_pcode(const std::string &label) {
    if (!over_exp)
        code_vec.push_back(std::make_shared<pcode>(label));
}

void parser::print_pcode() {
    std::ofstream ofs;
    ofs.open("pcode.txt");
    int cnt = 0;
    for (auto &pc: code_vec) {
        cnt++;
        ofs << pc->to_string() << std::endl;
    }
    ofs.close();
}

#pragma clang diagnostic pop