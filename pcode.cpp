//
// Created by Administrator on 2022/11/2.
//

#include "pcode.h"
using tp = table::pcode;

string pcode::pcode_to_string(table::pcode p) {
    std::string str;
    switch (p) {
        case table::pcode::LOAD:
            str = "LOAD";
            break;
        case table::pcode::LOADI:
            str = "LOADI";
            break;
        case table::pcode::STO:
            str = "STO";
            break;
        case table::pcode::STORIN:
            str = "STORIN";
            break;
        case table::pcode::ADD:
            str = "ADD";
            break;
        case table::pcode::SUB:
            str = "SUB";
            break;
        case table::pcode::MULT:
            str = "MULT";
            break;
        case table::pcode::DIV:
            str = "DIV";
            break;
        case table::pcode::BR:
            str = "BR";
            break;
        case table::pcode::BRF:
            str = "BRF";
            break;
        case table::pcode::EQ:
            str = "EQ";
            break;
        case table::pcode::NEQ:
            str = "NEQ";
            break;
        case table::pcode::GEQ:
            str = "GEQ";
            break;
        case table::pcode::LEQ:
            str = "LEQ";
            break;
        case table::pcode::GT:
            str = "GT";
            break;
        case table::pcode::LES:
            str = "LES";
            break;
        case table::pcode::AND:
            str = "AND";
            break;
        case table::pcode::OR:
            str = "OR";
            break;
        case table::pcode::ANDU:
            str = "ANDU";
            break;
        case table::pcode::ORU:
            str = "ORU";
            break;
        case table::pcode::ANDEND:
            str = "ANDEND";
            break;
        case table::pcode::OREND:
            str = "OREND";
            break;
        case table::pcode::NOT:
            str = "NOT";
            break;
        case table::pcode::SETD1:
            str = "SETD1";
            break;
        case table::pcode::SETD2:
            str = "SETD2";
            break;
        case table::pcode::J:
            str = "J";
            break;
        case table::pcode::RE:
            str = "RE";
            break;
        case table::pcode::GETINT:
            str = "GETINT";
            break;
        case table::pcode::PRINT:
            str = "PRINT";
            break;
        case table::pcode::OUTB:
            str = "OUTB";
            break;
        case table::pcode::NEW:
            str = "NEW";
            break;
        case table::pcode::BOUND1:
            str = "BOUND1";
            break;
        case table::pcode::BOUND2:
            str = "BOUND2";
            break;
        case table::pcode::SETD1R:
            str = "SETD1R";
            break;
        case table::pcode::SETD2R:
            str = "SETD2R";
            break;
        case table::pcode::FRP:
            str = "FRP";
            break;
        case table::pcode::PARAMEND:
            str = "PARAMEND";
            break;
        case table::pcode::FUNCDEF:
            str = "FUNCDEF";
            break;
        case table::pcode::ELEM:
            str = "ELEM";
            break;
        case table::pcode::END:
            str = "END";
            break;
        case table::pcode::LABEL:
            str = "LABEL";
            break;
        case table::pcode::INB:
            str = "INB";
            break;
        case table::pcode::NEG:
            str = "NEG";
            break;
        case table::pcode::TYPE:
            str = "TYPE";
            break;
        case table::pcode::ANDUNIT:
            str = "ANDUNIT";
            break;
        case table::pcode::ORUNIT:
            str = "ORUNIT";
            break;
        case table::pcode::MOD:
            str = "MOD";
            break;
        case table::pcode::POS:
            str = "POS";
            break;
        case table::pcode::BITAND:
            str = "BITAND";
    }
    return str;
}

string pcode::to_string() {
    std::string pcode_string, str;
    pcode_string = pcode_to_string(pcode_type);
    switch (pcode_type) {
        case tp::NEW:
        case tp::LOAD:
        case tp::STO:
            if (idptr->get_ident_type() != table::identifier::UNDEF) {
                str = pcode_string + " " + idptr->get_ident().get_name();
            }
            break;
        case tp::LABEL:
            str = label + ":";
            break;
        case tp::LOADI:
            str = pcode_string + " " + std::to_string(instant);
            break;
        case tp::BR:
        case tp::BRF:
        case tp::J:
            str = pcode_string + " " + label;
            break;
        case tp::RE:
            if (!label.empty()) {
                str = pcode_string + " " + label;
            }
        case tp::PRINT:
            if (!label.empty()) {
                str = pcode_string + " " + label;
            } else {
                str = pcode_string + " EXP";
            }
            break;
        default:
            str = pcode_string;
            break;
    }
    return str;
}
