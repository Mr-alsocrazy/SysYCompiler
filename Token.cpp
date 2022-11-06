//
// Created by Administrator on 2022/9/12.
//

#include "Token.h"

string Token::token_str() const {
    string str;
    if (!name.empty()) {
        switch (get_symcode()) {
            case table::sym::IDENFR:
                str = "IDENFR " + name;
                break;
            case table::sym::STRCON:
                str = "STRCON " + name;
                break;
            default: {
            }
        }
    } else {
        switch (get_symcode()) {
            case table::sym::INTCON:
                str = "INTCON " + std::to_string(value);
                break;
            case table::sym::PRINTFTK:
                str = "PRINTFTK printf";
                break;
            case table::sym::MAINTK:
                str = "MAINTK main";
                break;
            case table::sym::CONSTTK:
                str = "CONSTTK const";
                break;
            case table::sym::INTTK:
                str = "INTTK int";
                break;
            case table::sym::BREAKTK:
                str = "BREAKTK break";
                break;
            case table::sym::CONTINUETK:
                str = "CONTINUETK continue";
                break;
            case table::sym::IFTK:
                str = "IFTK if";
                break;
            case table::sym::ELSETK:
                str = "ELSETK else";
                break;
            case table::sym::NOT:
                str = "NOT !";
                break;
            case table::sym::AND:
                str = "AND &&";
                break;
            case table::sym::OR:
                str = "OR ||";
                break;
            case table::sym::WHILETK:
                str = "WHILETK while";
                break;
            case table::sym::GETINTTK:
                str = "GETINTTK getint";
                break;
            case table::sym::RETURNTK:
                str = "RETURNTK return";
                break;
            case table::sym::PLUS:
                str = "PLUS +";
                break;
            case table::sym::MINU:
                str = "MINU -";
                break;
            case table::sym::VOIDTK:
                str = "VOIDTK void";
                break;
            case table::sym::MULT:
                str = "MULT *";
                break;
            case table::sym::DIV:
                str = "DIV /";
                break;
            case table::sym::MOD:
                str = "MOD %";
                break;
            case table::sym::LSS:
                str = "LSS <";
                break;
            case table::sym::LEQ:
                str = "LEQ <=";
                break;
            case table::sym::GRE:
                str = "GRE >";
                break;
            case table::sym::GEQ:
                str = "GEQ >=";
                break;
            case table::sym::EQL:
                str = "EQL ==";
                break;
            case table::sym::NEQ:
                str = "NEQ !=";
                break;
            case table::sym::ASSIGN:
                str = "ASSIGN =";
                break;
            case table::sym::SEMICN:
                str = "SEMICN ;";
                break;
            case table::sym::COMMA:
                str = "COMMA ,";
                break;
            case table::sym::LPARENT:
                str = "LPARENT (";
                break;
            case table::sym::RPARENT:
                str = "RPARENT )";
                break;
            case table::sym::LBRACK:
                str = "LBRACK [";
                break;
            case table::sym::RBRACK:
                str = "RBRACK ]";
                break;
            case table::sym::LBRACE:
                str = "LBRACE {";
                break;
            case table::sym::RBRACE:
                str = "RBRACE }";
                break;
            case table::sym::NONETYPE:
                break;
            default: {
            }
        }
    }
    str += (" " + std::to_string(line));
    return str;
}