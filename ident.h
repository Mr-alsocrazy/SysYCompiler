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
    shared_ptr<Token> ident_ptr;
    bool is_const;
    table::identifier identype;
public:
    ident(shared_ptr<Token> _ident_ptr, bool _is_const) : ident_ptr(std::move(_ident_ptr)), is_const(_is_const) {}

    shared_ptr<Token> get_ident() const { return ident_ptr; }

    bool get_is_const() const { return is_const; }

    bool operator==(const ident &I) const;

    void set_ident_type(table::identifier t) { identype = t; }

    table::identifier get_ident_type() const { return identype; }

    static table::identifier get_ident_type(int t);
};


#endif //LEXICALANALYSIS_IDENT_H
