//
// Created by Administrator on 2022/10/21.
//

#include "ident.h"
using tid = table::identifier;

bool ident::operator== (const ident& I) const {
    return (ident_ptr->get_name() == I.get_ident()->get_name()
            && ident_ptr->get_symcode() == I.get_ident()->get_symcode());
}

tid ident::get_ident_type(int t) {
    switch (t) {
        case 0:
            return tid::INTEGER;
        case 1:
            return tid::ONE_DIMENSION_ARRAY;
        case 2:
            return tid::TWO_DIMENSION_ARRAY;
        default:
            return tid::UNDEF;
    }
}
