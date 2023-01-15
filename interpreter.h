//
// Created by Administrator on 2022/11/3.
//
//#define LEXICALANALYSIS_INTERPRETER_H
#ifndef LEXICALANALYSIS_INTERPRETER_H
#define LEXICALANALYSIS_INTERPRETER_H

#include <iostream>
#include <vector>
#include <memory>
#include "ident.h"
#include "parser.h"

class interpreter {
private:
    size_t main_idx;
    std::vector<ident> opr;
    std::vector<std::vector<ident>> itbl;
    std::vector<bool> array_assign;
    std::vector<int> idx_list;
    std::vector<int> dimension_1, dimension_2;
    std::vector<std::string> output;
    size_t cur_idx;
    table::identifier cur_ident_type;
    bool true_exist;
    bool false_exist;
    int true_cnt = 0;
    int false_cnt = 0;
public:
    explicit interpreter(size_t _main_idx) : main_idx(_main_idx), cur_idx(0), true_exist(false), false_exist(false),
                                                true_cnt(0), false_cnt(0){
        dimension_1.push_back(-1);
        dimension_2.push_back(-1);
        itbl.emplace_back();
        array_assign.push_back(false);
        cur_ident_type = table::identifier::UNDEF;
    }

    void interpret();

    void load_ident();

    void sto(ident &id);

    void storin();

    void load(const ident &id);

    void loadi(int instant);

    void br(std::string &label);

    void brf(std::string &label);

    void j(std::string &label);

    void add();

    void sub();

    void neg();

    void pos();

    void mult();

    void div();

    void mod();

    void bitandc();

    void andc();

    void orc();

    void notc();

    void eq();

    void neq();

    void geq();

    void leq();

    void gt();

    void les();

    void and_unit();

    void or_unit();

    void and_end();

    void or_end();

    void scan();

    void print();

    void setd1();

    void setd2();

    void inb();

    void outb();

    void newvar(ident &id);

    void set_bound1(const ident &id);

    void set_bound2(const ident &id);

    void elem();

    void setd1r();

    void setd2r();

    void re();

    void set_type(table::identifier t) { cur_ident_type = t; }

    ident get_ident(const ident& id);

    void set_ident(const ident& id);

    bool update_ident(const ident& id);

    ident ident_from_stack(ident& id);

    void print_result();
};


#endif //LEXICALANALYSIS_INTERPRETER_H
