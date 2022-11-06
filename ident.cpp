//
// Created by Administrator on 2022/10/21.
//

#include "ident.h"

using tid = table::identifier;

bool ident::operator==(const ident &I) const {
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

int ident::get_value(int d1, int d2) const {
    if (d1 != -1) {
        if (d2 == -1)
            return get_arr_value(d1);
        else
            return get_arr_value(d1, d2);
    } else
        return ident_ptr->get_value();
}

int ident::get_value() const {
    if (dimen1 != -1) {
        if (dimen2 == -1)
            return get_arr_value(dimen1);
        else
            return get_arr_value(dimen1, dimen2);
    } else
        return ident_ptr->get_value();
}

void ident::set_value(int value, int d1, int d2) {
    if (d1 != -1) {
        if (d2 == -1)
            one_dim_array.at(d1) = value;
        else
            two_dim_array.at(d1).at(d2) = value;
    } else
        ident_ptr->set_value(value);
}

void ident::set_value(int value) {
    if (dimen1 != -1) {
        if (dimen2 == -1)
            one_dim_array.at(dimen1) = value;
        else
            two_dim_array.at(dimen1).at(dimen2) = value;
    } else
        ident_ptr->set_value(value);
}

const vector<std::string> &ident::get_used_name() const {
    return used_name;
}

void ident::add_element(int e) {
    if (identype == tid::ONE_DIMENSION_ARRAY) {
        one_dim_array.at(index1 + offset1) = e;
        index1++;
    } else if (identype == tid::TWO_DIMENSION_ARRAY) {
        two_dim_array[index2][index1] = e;
        index1++;
        if (index1 == bound2) {
            index1 = 0;
            index2++;
        }
    }
}

void ident::set_bound1(int b1) {
    bound1 = b1;
    if (identype == tid::ONE_DIMENSION_ARRAY) {
        for (int i = 0; i < bound1; i++)
            one_dim_array.push_back(0);
    } else if (identype == tid::TWO_DIMENSION_ARRAY) {
        for (int i = 0; i < bound1; i++)
            two_dim_array.emplace_back();
    }
}

void ident::set_bound2(int b2) {
    bound2 = b2;
    for (int i = 0; i < bound1; i++)
        for (int j = 0; j < bound2; j++)
            two_dim_array[i][j] = 0;
}

void ident::set_used_name(const vector<std::string> &_used_name) {
    used_name = _used_name;
}
