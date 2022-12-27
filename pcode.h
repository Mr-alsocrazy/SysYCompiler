//
// Created by Administrator on 2022/11/2.
//

#ifndef LEXICALANALYSIS_PCODE_H
#define LEXICALANALYSIS_PCODE_H
#include "tables.h"
#include "ident.h"
#include <memory>
using tp = table::pcode;

class pcode {
private:
    tp pcode_type;
    shared_ptr<ident> idptr;
    int instant{};
    string label;
public:
    pcode (tp _pcode_type, shared_ptr<ident> _ident): pcode_type(_pcode_type), idptr(std::move(_ident)) {}

    pcode (tp _pcode_type, int _instant): pcode_type(_pcode_type), instant(_instant) {
        idptr = std::make_shared<ident>(table::identifier::UNDEF);
    }

    pcode (tp _pcode_type, string _label): pcode_type(_pcode_type), label(std::move(_label)) {
        idptr = std::make_shared<ident>(table::identifier::UNDEF);
    }

    explicit pcode (string _label): pcode_type(table::pcode::LABEL), label(std::move(_label)) {
        idptr = std::make_shared<ident>(table::identifier::UNDEF);
    }

    explicit pcode (tp _pcode_type): pcode_type(_pcode_type) {
        idptr = std::make_shared<ident>(table::identifier::UNDEF);
    }

    std::shared_ptr<ident> get_idptr() { return idptr; }

    int get_instant() const { return instant; }

    void set_instant(int _instant) { pcode::instant = _instant; }

    const string &get_label() const { return label; }

    void set_label(const string &_label) { pcode::label = _label; }

    void set_ident_tpye(table::identifier id) { idptr->set_ident_type(id); }

    tp get_ptype() const { return pcode_type; }

    std::shared_ptr<ident> get_ident() const {
        return idptr;
    }

    static string pcode_to_string(tp tp1);

    string to_string();
};


#endif //LEXICALANALYSIS_PCODE_H
