//
// Created by Administrator on 2022/10/21.
//

#ifndef LEXICALANALYSIS_IDENT_H
#define LEXICALANALYSIS_IDENT_H

#include "Token.h"
#include "tables.h"
#include <memory>
#include <utility>

class ident {
private:
    Token id;
    bool is_const;
    table::identifier identype;
public:
    ident(Token _ident_ptr, bool _is_const) : id(std::move(_ident_ptr)), is_const(_is_const) {
        decl();
    }

    explicit ident(Token _ident_ptr) : id(std::move(_ident_ptr)) {
        decl();
        is_const = false;
    }

    explicit ident(table::identifier _identype) : identype(_identype) {
        decl();
        id = Token();
        is_const = false;
    }

    explicit ident(int instant) {
        decl();
        id = Token(table::sym::INTCON, instant, 0);
        is_const = true;
        identype = table::identifier::INTEGER;
    }

    void decl() {
        dimen1 = -1;
        dimen2 = -1;
        bound1 = 0;
        bound2 = 0;
        index1 = 0;
        index2 = 0;
        offset1 = 0;
        offset2 = 0;
        oitindex = -1;
    }

    Token get_ident() const { return id; }

    bool get_is_const() const { return is_const; }

    bool operator==(const ident &I) const;

    void set_ident_type(table::identifier t) { identype = t; }

    table::identifier get_ident_type() const { return identype; }

    static table::identifier get_ident_type(int t);

private:
    int dimen1, dimen2, bound1, bound2, index1, index2, oitindex;
    std::vector<std::string> used_name;
public:
    void set_used_name(const vector<std::string> &_used_name);

public:
    const vector<std::string> &get_used_name() const;

public:
    int get_bound1() const { return bound1; }

    int get_bound2() const { return bound2; }

    void set_bound1(int b1);

    void set_bound2(int b2);

    int get_value(int d1, int d2) const;

    int get_value() const;

    int get_oit() const { return oitindex; }

    void set_oit(int o) { oitindex = o; }

    void add_used_name(const string &_) { used_name.push_back(_); };

private:
    std::vector<int> one_dim_array;
    std::vector<std::vector<int>> two_dim_array;
    int offset1, offset2;
public:
    int get_offset1() const { return offset1; }

    void set_offset1(int _) { ident::offset1 = _; }

    std::vector<int> get_1darray() { return one_dim_array; }

    void set_1darray(std::vector<int> _) { one_dim_array = std::move(_); }

    std::vector<std::vector<int>> get_2darray() { return two_dim_array; }

    void set_2darray(std::vector<std::vector<int>> _) { two_dim_array = std::move(_); }

private:
    size_t location;
public:

    void set_dim1_dim2(std::vector<int> vec, int i) {
        two_dim_array.at(i) = std::move(vec);
    }

    vector<int> get1d() const { return one_dim_array; }

    vector<vector<int>> get2d() const { return two_dim_array; }

    void set1d(vector<int> d) { one_dim_array = std::move(d); }

    void set2d(vector<vector<int>> d) { two_dim_array = std::move(d); }

    int get_arr_value(int d1) const { return one_dim_array.at(d1); }

    int get_arr_value(int d1, int d2) const { return two_dim_array.at(d1).at(d2); }

    void set_value(int value);

    void set_value(int value, int d1, int d2);

    void set_dimen1(int value) { dimen1 = value; }

    void set_dimen2(int value) { dimen2 = value; }

    int get_dimen1() const { return dimen1; }

    int get_dimen2() const { return dimen2; }

    void set_location(int l) { location = l; }

    size_t get_location() const { return location; }

    void add_element(int e);
};


#endif //LEXICALANALYSIS_IDENT_H
