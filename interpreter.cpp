//
// Created by Administrator on 2022/11/3.
//
//#define DEBUG
//#define __PRINT__
#ifndef DEBUG
#define PCODE

#include "interpreter.h"

using tp = tp;
using tid = table::identifier;
using tsy = table::sym;
bool elem_assign = false;
int func_layer = 0;
bool param = false;
bool funcr = false;
std::vector<::size_t> j_idx;
std::vector<int> block_idx;
int step = 1;

void foo() {
    return;
};

void interpreter::interpret() {
    ofstream ofs_track;
#ifdef PCODE
    ofs_track.open("track.txt");
#endif
    for (cur_idx = 0; cur_idx < code_vec.size() && code_vec.at(cur_idx)->get_ptype() != tp::END; cur_idx++) {
        step++;
#ifdef PCODE
        ofs_track << std::to_string(cur_idx) + " : " + code_vec.at(cur_idx)->to_string() << endl;
#endif
        tp type = code_vec.at(cur_idx)->get_ptype();
        int value = code_vec.at(cur_idx)->get_instant();
        auto idptr = code_vec.at(cur_idx)->get_ident();
        tid idtype = code_vec.at(cur_idx)->get_ident()->get_ident_type();
        string label = code_vec.at(cur_idx)->get_label();
        if (step == 84) {
            foo();
        }
        switch (type) {
            case tp::LOAD:
                load(*idptr);
                break;
            case tp::LOADI:
                loadi(value);
                break;
            case tp::STO:
                sto(*idptr);
                break;
            case tp::STORIN:
                storin();
                break;
            case tp::ADD:
                add();
                break;
            case tp::SUB:
                sub();
                break;
            case tp::NEG:
                neg();
                break;
            case tp::POS:
                pos();
                break;
            case tp::MULT:
                mult();
                break;
            case tp::DIV:
                div();
                break;
            case tp::MOD:
                mod();
                break;
            case tp::BITAND:
                bitandc();
                break;
            case tp::AND:
                andc();
                break;
            case tp::OR:
                orc();
                break;
            case tp::NOT:
                notc();
                break;
            case tp::ANDUNIT:
                and_unit();
                break;
            case tp::ORUNIT:
                or_unit();
                break;
            case tp::ANDEND:
                and_end();
                break;
            case tp::OREND:
                or_end();
                break;
            case tp::BR:
                br(label);
                break;
            case tp::BRF:
                brf(label);
                break;
            case tp::EQ:
                eq();
                break;
            case tp::NEQ:
                neq();
                break;
            case tp::GT:
                gt();
                break;
            case tp::GEQ:
                geq();
                break;
            case tp::LES:
                les();
                break;
            case tp::LEQ:
                leq();
                break;
            case tp::SETD1:
                setd1();
                break;
            case tp::SETD2:
                setd2();
                break;
            case tp::J:
                j(label);
                break;
            case tp::RE:
                re();
                break;
            case tp::GETINT:
                scan();
                break;
            case tp::PRINT:
                print();
                break;
            case tp::INB:
                inb();
                break;
            case tp::OUTB:
                outb();
                break;
            case tp::NEW:
                newvar(*idptr);
                break;
            case tp::BOUND1:
                set_bound1(*idptr);
                break;
            case tp::BOUND2:
                set_bound2(*idptr);
                break;
            case tp::SETD1R:
                setd1r();
                break;
            case tp::SETD2R:
                setd2r();
                break;
            case tp::TYPE:
                set_type(idtype);
                break;
            case tp::FRP:
                funcr = true;
                break;
            case tp::PARAMEND:
                param = false;
                break;
            case tp::FUNCDEF:
                cur_idx = main_idx;
                break;
            case tp::ELEM:
                elem();
                break;
            default:
                break;
        }
    }
}

void interpreter::sto(ident& id) {
    id = ident_from_stack(id);
    set_ident(id);
    opr.pop_back();
}

void interpreter::storin() {
    if (elem_assign) {
        update_ident(opr.back());
        opr.pop_back();
        elem_assign = false;
    } else {
        auto pop = opr.back();
        opr.pop_back();
        auto next = opr.back();
        opr.pop_back();
        next.set_value(pop.get_value(), dimension_1.back(), dimension_2.back());
        if (dimension_1.back() != -1 || dimension_2.back() != -1) {
            dimension_1.back() = -1;
            dimension_2.back() = -1;
        }
        set_ident(next);
    }
}

void interpreter::load(const ident &id) {
    ident load = get_ident(id);
    opr.push_back(load);
}

void interpreter::loadi(int instant) {
    opr.emplace_back(instant);
}

void interpreter::br(std::string &label) {
    size_t len = code_vec.size();
    for (size_t idx = 0; idx < len; idx++)
        if (code_vec.at(idx)->get_ptype() == tp::LABEL && code_vec.at(idx)->get_label() == label) {
            cur_idx = idx;
            break;
        }
}

void interpreter::brf(std::string &label) {
    if (opr.back().get_value() == 0)
        br(label);
    opr.pop_back();
}

void interpreter::j(string &label) {
    j_idx.push_back(cur_idx + 1);
    br(label);
    block_idx.push_back(0);
    func_layer++;
    funcr = false;
    param = true;
    array_assign.push_back(false);
    dimension_1.push_back(-1);
    dimension_2.push_back(-1);
}

void interpreter::add() {
    auto pop = opr.back();
    auto next = opr.at(opr.size() - 2);
    if (funcr && cur_ident_type != tid::INTEGER &&
        !(next.get_ident_type() != tid::INTEGER && pop.get_ident_type() != tid::INTEGER)) {
        opr.pop_back();
        opr.pop_back();
        switch (cur_ident_type) {
            case tid::ONE_DIMENSION_ARRAY:
                if (pop.get_ident_type() != tid::ONE_DIMENSION_ARRAY && next.get_ident_type() == tid::INTEGER) {
                    ident &in = pop;
                    in.set_bound1(in.get_bound1() + next.get_value());
                    opr.push_back(in);
                } else {
                    ident &in = next;
                    in.set_bound1(in.get_bound1() + next.get_value());
                    opr.push_back(in);
                }
                break;
            case tid::TWO_DIMENSION_ARRAY:
                if (pop.get_ident_type() != tid::TWO_DIMENSION_ARRAY && next.get_ident_type() == tid::INTEGER) {
                    ident &in = pop;
                    in.set_bound1(in.get_bound1() + next.get_value());
                    opr.push_back(in);
                } else {
                    ident &in = next;
                    in.set_bound1(in.get_bound1() + next.get_value());
                    opr.push_back(in);
                }
                break;
            default:
                break;
        }
    } else {
        opr.pop_back();
        opr.pop_back();
        auto _next = next;
        if (next.get_ident().nonToken && next.get_value() == 0) {
            _next = get_ident(next);
            _next.set_dimen1(next.get_dimen1());
            _next.set_dimen2(next.get_dimen2());
        }
        auto _pop = pop;
        if (pop.get_ident().nonToken && pop.get_value() == 0) {
            _pop = get_ident(pop);
            _pop.set_dimen1(pop.get_dimen1());
            _pop.set_dimen2(pop.get_dimen2());
        }
        ident res(_pop.get_value() + _next.get_value());
        opr.emplace_back(res);
    }
}

void interpreter::sub() {
    auto pop = opr.back();
    auto next = opr.at(opr.size() - 2);
    if (funcr && cur_ident_type != tid::INTEGER &&
        !(next.get_ident_type() != tid::INTEGER && pop.get_ident_type() != tid::INTEGER)) {
        opr.pop_back();
        opr.pop_back();
        switch (cur_ident_type) {
            case tid::ONE_DIMENSION_ARRAY:
                if (pop.get_ident_type() != tid::ONE_DIMENSION_ARRAY && next.get_ident_type() == tid::INTEGER) {
                    ident &in = pop;
                    in.set_bound1(in.get_bound1() - next.get_value());
                    opr.push_back(in);
                } else {
                    ident &in = next;
                    in.set_bound1(in.get_bound1() - next.get_value());
                    opr.push_back(in);
                }
                break;
            case tid::TWO_DIMENSION_ARRAY:
                if (pop.get_ident_type() != tid::TWO_DIMENSION_ARRAY && next.get_ident_type() == tid::INTEGER) {
                    ident &in = pop;
                    in.set_bound1(in.get_bound1() - next.get_value());
                    opr.push_back(in);
                } else {
                    ident &in = next;
                    in.set_bound1(in.get_bound1() - next.get_value());
                    opr.push_back(in);
                }
                break;
            default:
                break;
        }
    } else {
        opr.pop_back();
        opr.pop_back();
        ident res(next.get_value() - pop.get_value());
        opr.emplace_back(res);
    }
}

void interpreter::neg() {
    auto pop = opr.back();
    opr.pop_back();
    opr.emplace_back(-pop.get_value());
}

void interpreter::pos() {
    auto pop = opr.back();
    opr.pop_back();
    opr.emplace_back(pop.get_value());
}

void interpreter::mult() {
    auto pop = opr.back();
    auto next = opr.at(opr.size() - 2);
    opr.pop_back();
    opr.pop_back();
    ident res(pop.get_value() * next.get_value());
    opr.emplace_back(res);
}

void interpreter::div() {
    auto pop = opr.back();
    auto next = opr.at(opr.size() - 2);
    opr.pop_back();
    opr.pop_back();
    ident res(next.get_value() / pop.get_value());
    opr.emplace_back(res);
}

void interpreter::mod() {
    auto pop = opr.back();
    auto next = opr.at(opr.size() - 2);
    opr.pop_back();
    opr.pop_back();
    ident res(next.get_value() % pop.get_value());
    opr.emplace_back(res);
}

void interpreter::bitandc() {
    auto pop = opr.back();
    auto next = opr.at(opr.size() - 2);
    opr.pop_back();
    opr.pop_back();
    ident res(next.get_value() & pop.get_value());
    opr.emplace_back(res);
}

void interpreter::andc() {
    auto pop = opr.back();
    auto next = opr.at(opr.size() - 2);
    opr.pop_back();
    opr.pop_back();
    ident res(next.get_value() && pop.get_value());
    opr.emplace_back(res);
}

void interpreter::orc() {
    auto pop = opr.back();
    auto next = opr.at(opr.size() - 2);
    opr.pop_back();
    opr.pop_back();
    ident res(next.get_value() || pop.get_value());
    opr.emplace_back(res);
}

void interpreter::notc() {
    auto pop = opr.back();
    opr.pop_back();
    opr.emplace_back(!pop.get_value());
}

void interpreter::eq() {
    auto pop = opr.back();
    auto next = opr.at(opr.size() - 2);
    opr.pop_back();
    opr.pop_back();
    ident res(next.get_value() == pop.get_value());
    opr.emplace_back(res);
}

void interpreter::neq() {
    auto pop = opr.back();
    auto next = opr.at(opr.size() - 2);
    opr.pop_back();
    opr.pop_back();
    ident res(next.get_value() != pop.get_value());
    opr.emplace_back(res);
}

void interpreter::geq() {
    auto pop = opr.back();
    auto next = opr.at(opr.size() - 2);
    opr.pop_back();
    opr.pop_back();
    ident res(next.get_value() >= pop.get_value());
    opr.emplace_back(res);
}

void interpreter::leq() {
    auto pop = opr.back();
    auto next = opr.at(opr.size() - 2);
    opr.pop_back();
    opr.pop_back();
    ident res(next.get_value() <= pop.get_value());
    opr.emplace_back(res);
}

void interpreter::gt() {
    auto pop = opr.back();
    auto next = opr.at(opr.size() - 2);
    opr.pop_back();
    opr.pop_back();
    ident res(next.get_value() > pop.get_value());
    opr.emplace_back(res);
}

void interpreter::les() {
    auto pop = opr.back();
    auto next = opr.at(opr.size() - 2);
    opr.pop_back();
    opr.pop_back();
    ident res(next.get_value() < pop.get_value());
    opr.emplace_back(res);
}

void interpreter::or_unit() {
    if (opr.back().get_value() != 0) {
        true_exist = true;
        cur_idx++;
        while (code_vec.at(cur_idx)->get_ptype() != tp::OREND)
            cur_idx++;
        cur_idx--;
    } else {
        false_cnt++;
        if (false_cnt > 1)
            cur_idx++;
    }
}

void interpreter::and_unit() {
    if (opr.back().get_value() == 0) {
        false_exist = true;
        cur_idx++;
        while (code_vec.at(cur_idx)->get_ptype() != tp::ANDEND)
            cur_idx++;
        cur_idx--;
    } else {
        true_cnt++;
        if (true_cnt > 1)
            cur_idx++;
    }
}

void interpreter::and_end() {
    if (!false_exist) {
        while (true_cnt > 1) {
            andc();
            true_cnt--;
        }
    } else {
        while (true_cnt > 0) {
            andc();
            true_cnt--;
        }
        false_exist = false;
    }
    true_cnt = 0;
}

void interpreter::or_end() {
    if (!true_exist) {
        while (false_cnt > 1) {
            orc();
            false_cnt--;
        }
    } else {
        while (false_cnt > 0) {
            orc();
            false_cnt--;
        }
        true_exist = false;
    }
    false_cnt = 0;
}

void interpreter::scan() {
    int value;
    cin >> value;
    loadi(value);
}

void interpreter::print() {
    int argc = 0;
    while (code_vec.at(cur_idx)->get_ptype() == tp::PRINT) {
        if (code_vec.at(cur_idx)->get_label().empty()) {
            int num = code_vec.at(cur_idx)->get_instant();
            string numstr = std::to_string(opr.at(opr.size() - num).get_value());
            output.push_back(numstr);
            argc++;
#ifdef __PRINT__
            std::cout << numstr << std::endl;
#endif
        } else if (code_vec.at(cur_idx)->get_label() == "\\n") {
            output.emplace_back("\n");
#ifdef __PRINT__
            std::cout << "\n" << std::endl;
#endif
        } else {
            output.push_back(code_vec.at(cur_idx)->get_label());
#ifdef __PRINT__
            std::cout << code_vec.at(cur_idx)->get_label() << std::endl;
#endif
        }
        cur_idx++;
    }
    for (int i = 0; i < argc; i++)
        opr.pop_back();
    cur_idx--;
}

void interpreter::setd1() {
    array_assign.back() = true;
    dimension_1.back() = opr.back().get_value();
    opr.pop_back();
}

void interpreter::setd2() {
    dimension_2.back() = opr.back().get_value();
    opr.pop_back();
}

void interpreter::inb() {
    itbl.emplace_back();
    cout << std::to_string(step) + ":emplace:" << itbl.size() << endl;
    if (func_layer > 0 && !block_idx.empty())
        block_idx.back()++;
}

void interpreter::outb() {
    itbl.pop_back();
    cout << to_string(step) + ":pop:"<< itbl.size() << endl;
    if (!block_idx.empty())
        block_idx.back()--;
}

void interpreter::newvar(ident &id) {
    if (id.get_ident_type() == tid::ONE_DIMENSION_ARRAY or id.get_ident_type() == tid::TWO_DIMENSION_ARRAY)
        id.set_location(itbl.size());
    opr.push_back(id);
    itbl.back().push_back(id);
}

ident interpreter::get_ident(const ident& id) {
    if (func_layer == 0) {
        ident res(tid::UNDEF);
        for (auto iter1 = itbl.rbegin(); iter1 != itbl.rend(); iter1++) {
            for (auto iter2 = (*iter1).rbegin(); iter2 != (*iter1).rend(); iter2++)
                if (*iter2 == id) {
                    res = *iter2;
                    return res;
                }
        }
        return res;
    } else {
        ident res(tid::UNDEF);
        int max_range = 0;
        if (!block_idx.empty())
            max_range = block_idx.back();
        for (auto iter1 = itbl.rbegin(); iter1 != itbl.rend() and max_range > 0; iter1++) {
            for (auto &iter2: *iter1)
                if (iter2 == id) {
//                    iter2 = id;
                    res = iter2;
                    return res;
                }
            max_range--;
        }
        for (auto & it : itbl.at(0))
            if (it == id) {
                res = it;
                return res;
            }
        return res;
    }
    return ident(tid::UNDEF);
}

bool interpreter::update_ident(const ident &id) {
    bool res = false;
    if (!array_assign.empty() && array_assign.back() && func_layer != 0) {
        array_assign.back() = false;
        int max_range = (!block_idx.empty()) ? block_idx.back() : 0;
        int loc = id.get_location() - 1;
        auto arr_name = id.get_used_name();
        bool result = false;
        for (int i = itbl.size() - 1; i >= loc; i--) {
            int len = itbl.at(i).size();
            for (int j = 0; j < len; j++)
                if (max_range > 0) {
                    if (itbl[i][j] == id) {
                        itbl[i][j] = id;
                        result = true;
                    }
                } else {
                    if (!arr_name.empty() and itbl[i][j].get_ident().get_name() == arr_name.back()) {
                        arr_name.pop_back();
                        if (itbl[i][j].get_ident_type() == tid::TWO_DIMENSION_ARRAY &&
                            id.get_ident_type() == tid::ONE_DIMENSION_ARRAY)
                            itbl[i][j].set_dim1_dim2(id.get1d(), id.get_oit());
                        else if (id.get_ident_type() == tid::ONE_DIMENSION_ARRAY)
                            itbl[i][j].set1d(id.get1d());
                        else
                            itbl[i][j].set2d(id.get2d());
                        result = true;
                    }
                }
            max_range--;
        }
        for (auto & it : itbl.at(0))
            if (it == id) {
                it = id;
                return true;
            }
    } else if (func_layer == 0) {
        for (auto it1 = itbl.rbegin(); it1 != itbl.rend(); it1++) {
            for (auto & it2 : *it1)
                if (it2 == id) {
                    it2 = id;
                    res = true;
                    return res;
                }
        }
        return res;
    } else {
        int max_range = (!block_idx.empty()) ? block_idx.back() : 0;
        for (auto it1 = itbl.rbegin(); it1 != itbl.rend() && max_range > 0; it1++) {
            for (auto & it2 : *it1)
                if (it2 == id) {
                    it2 = id;
                    res = true;
                    return res;
                }
            max_range--;
        }
        for (auto & it : itbl.at(0))
            if (it == id) {
                it = id;
                return true;
            }
    }
    return res;
}

void interpreter::set_ident(const ident& id) {
    if (!update_ident(id))
        itbl.back().push_back(id);
}

void interpreter::set_bound1(const ident& id) {
    opr.at(opr.size() - 2).set_bound1(opr.at(opr.size() - 1).get_value());
    update_ident(opr.at(opr.size() - 2));
    opr.pop_back();
}

void interpreter::set_bound2(const ident& id) {
    opr.at(opr.size() - 2).set_bound2(opr.at(opr.size() - 1).get_value());
    update_ident(opr.at(opr.size() - 2));
    opr.pop_back();
}

void interpreter::elem() {
    opr.at(opr.size() - 2).add_element(opr.at(opr.size() - 1).get_value());
    opr.pop_back();
    elem_assign = true;
}

void interpreter::setd1r() {
    size_t cur = opr.size();
    opr.at(cur - 2).set_dimen1(opr.at(cur - 1).get_value());
    opr.pop_back();
}

void interpreter::setd2r() {
    size_t cur = opr.size();
    opr.at(cur - 2).set_dimen2(opr.at(cur - 1).get_value());
    opr.pop_back();
}

void interpreter::re() {
    int cnt = block_idx.empty() ? 0 : block_idx.back();
    while (cnt > 0) {
        itbl.pop_back();
        cout << to_string(step) + ":pop:"<< itbl.size() << endl;
        cnt--;
    }
    if (!block_idx.empty()) {
        block_idx.pop_back();
    }
    func_layer--;
    if (!array_assign.empty())
        array_assign.pop_back();
    if (code_vec.at(cur_idx)->get_label() != "VOID")
        opr.back().get_ident().snt(true);
    cur_idx = j_idx.back() - 1;
    j_idx.pop_back();
    dimension_1.pop_back();
    dimension_2.pop_back();
}

ident interpreter::ident_from_stack(ident& id) {
    auto store = opr.back();
    if (param) {
        if (store.get_ident_type() == tid::TWO_DIMENSION_ARRAY &&
            id.get_ident_type() == tid::ONE_DIMENSION_ARRAY) {
            id.set_used_name(store.get_used_name());
            id.set_oit(store.get_dimen1());
            id.set_offset1(store.get_offset1());
            id.set_bound1(store.get_bound2());
            id.set_1darray(store.get_2darray().at(store.get_dimen1()));
            id.add_used_name(store.get_ident().get_name());
        } else if (store.get_ident_type() == tid::ONE_DIMENSION_ARRAY &&
                   id.get_ident_type() == tid::ONE_DIMENSION_ARRAY) {
            id.set_used_name(store.get_used_name());
            id.set_offset1(store.get_offset1());
            id.set_bound1(store.get_bound2());
            id.set_1darray(store.get_1darray());
            id.add_used_name(store.get_ident().get_name());
        } else if (store.get_ident_type() == tid::TWO_DIMENSION_ARRAY &&
                   id.get_ident_type() == tid::TWO_DIMENSION_ARRAY) {
            id.set_used_name(store.get_used_name());
            id.set_offset1(store.get_offset1());
            id.set_bound1(store.get_bound2());
            id.set_bound2(store.get_bound2());
            id.set_2darray(store.get_2darray());
            id.add_used_name(store.get_ident().get_name());
        } else {
            switch (id.get_ident_type()) {
                case tid::INTEGER:
                    id.set_value(store.get_value());
                    break;
                case tid::ONE_DIMENSION_ARRAY:
                    id.set_bound1(store.get_bound1());
                    id.set_1darray(store.get_1darray());
                    break;
                case tid::TWO_DIMENSION_ARRAY:
                    id.set_bound1(store.get_bound1());
                    id.set_bound2(store.get_bound2());
                    id.set_1darray(store.get_1darray());
                    break;
                default:
                    break;
            }
        }
    } else {
        switch (store.get_ident_type()) {
            case tid::INTEGER:
                id.set_value(store.get_value());
                break;
            case tid::ONE_DIMENSION_ARRAY:
                id.set_bound1(store.get_bound1());
                id.set_1darray(store.get_1darray());
                break;
            case tid::TWO_DIMENSION_ARRAY:
                id.set_bound1(store.get_bound1());
                id.set_bound2(store.get_bound2());
                id.set_1darray(store.get_1darray());
                break;
            default:
                break;
        }
    }
    id.set_location(store.get_location());
    return id;
}

void interpreter::print_result() {
    ofstream ofs;
    ofs.open("pcoderesult.txt");
    for (auto & out : output)
        ofs << out;
    ofs.close();
}

#endif