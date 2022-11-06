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
    std::vector<shared_ptr<ident>> opr;
    std::vector<std::vector<shared_ptr<ident>>> itbl;
    std::vector<bool> array_assign;
    std::vector<int> idx_list;
    std::vector<int> dimension_1, dimension_2;
    std::vector<std::string> output;
    size_t cur_idx;
    table::identifier cur_ident_type;
public:
    explicit interpreter(size_t _main_idx) : main_idx(_main_idx), cur_idx(0) {
        dimension_1.push_back(-1);
        dimension_2.push_back(-1);
        itbl.emplace_back();
        array_assign.push_back(false);
        cur_ident_type = table::identifier::UNDEF;
    }

    void interpret();

    void load_ident();

    void sto(std::shared_ptr<ident> idptr);

    void storin();

    void load(const std::shared_ptr<ident> &idptr);

    void loadi(int instant);

    void br(std::string &label);

    void j(std::string &label);

    void add();

    void sub();

    void neg();

    void pos();

    void mult();

    void div();

    void mod();

    void scan();

    void print();

    void setd1();

    void setd2();

    void inb();

    void outb();

    void newvar(const std::shared_ptr<ident> &idptr);

    void set_bound1(const std::shared_ptr<ident> &idptr);

    void set_bound2(const std::shared_ptr<ident> &idptr);

    void elem();

    void setd1r();

    void setd2r();

    void re();

    void set_type(table::identifier t) { cur_ident_type = t; }

    std::shared_ptr<ident> get_ident(const std::shared_ptr<ident>& idptr);

    void set_ident(const std::shared_ptr<ident> &idptr);

    bool update_ident(const std::shared_ptr<ident>& idptr);

    std::shared_ptr<ident> ident_from_stack(std::shared_ptr<ident>);

    void print_result();
};


#endif //LEXICALANALYSIS_INTERPRETER_H
