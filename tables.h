//
// Created by Administrator on 2022/9/12.
//

#ifndef LEXICALANALYSIS_TABLES_H
#define LEXICALANALYSIS_TABLES_H
#include <vector>
#include <string>
#include <unordered_map>
using namespace std;

namespace table {
    enum class sym {
        IDENFR, INTCON, STRCON, MAINTK, CONSTTK,
        INTTK, BREAKTK, CONTINUETK, IFTK, ELSETK,
        NOT, AND, OR, WHILETK, GETINTTK, PRINTFTK,
        RETURNTK, PLUS, MINU, VOIDTK, MULT, DIV,
        MOD, LSS, LEQ, GRE, GEQ, EQL, NEQ, ASSIGN,
        SEMICN, COMMA, LPARENT, RPARENT, LBRACK,
        RBRACK, LBRACE, RBRACE, NONETYPE
    };

    enum class error_type {
        SYMBOL_ILLICIT,
        IDENFR_REDEF,
        IDENFR_UNDEF,
        PARA_NUM_NOT_MATCH,
        PARA_TYPE_NOT_MATCH,
        RETURN_NOT_MATCH,
        NO_RETURN,
        CONST_MODIFY,
        NO_SEMICN,
        NO_RPARENT,
        NO_RBRACK,
        PRINTF_PARA_NOT_MATCH,
        CONTINUE_BREAK_ERR
    };

    enum class pcode {
        LOAD, LOADI, STO, STORIN, ADD, POS, SUB, NEG, MULT, DIV, MOD, BR, BRF, ANDU, ORU, ANDEND, OREND,
        EQ, NEQ, GEQ, LEQ, GT, LES, AND, OR, NOT, SETD1, SETD2, J, RE, GETINT, PRINT, INB, OUTB, NEW, BOUND1,
        BOUND2, SETD1R, SETD2R, TYPE, FRP, PARAMEND, FUNCDEF, ANDUNIT, ORUNIT, LABEL, ELEM, END
    };

    enum class identifier {
        INTEGER, ONE_DIMENSION_ARRAY, TWO_DIMENSION_ARRAY, VOID, UNDEF
    };

    static unordered_map<error_type, string> error_table = {
            {error_type::SYMBOL_ILLICIT,        "a"}, // done
            {error_type::IDENFR_REDEF,          "b"}, // done
            {error_type::IDENFR_UNDEF,          "c"}, // done
            {error_type::PARA_NUM_NOT_MATCH,    "d"}, // done
            {error_type::PARA_TYPE_NOT_MATCH,   "e"}, // done
            {error_type::RETURN_NOT_MATCH,      "f"}, // done
            {error_type::NO_RETURN,             "g"}, // done
            {error_type::CONST_MODIFY,          "h"}, // done
            {error_type::NO_SEMICN,             "i"},
            {error_type::NO_RPARENT,            "j"},
            {error_type::NO_RBRACK,             "k"}, // done
            {error_type::PRINTF_PARA_NOT_MATCH, "l"}, // done
            {error_type::CONTINUE_BREAK_ERR,    "m"}  // done
    };

    static unordered_map<string, sym> reserved = {
        {"main",     sym::MAINTK},
        {"const",    sym::CONSTTK},
        {"int",      sym::INTTK},
        {"break",    sym::BREAKTK},
        {"continue", sym::CONTINUETK},
        {"if",       sym::IFTK},
        {"else",     sym::ELSETK},
        {"!",        sym::NOT},
        {"&&",       sym::AND},
        {"||",       sym::OR},
        {"while",    sym::WHILETK},
        {"getint",   sym::GETINTTK},
        {"printf",   sym::PRINTFTK},
        {"return",   sym::RETURNTK},
        {"+",        sym::PLUS},
        {"-",        sym::MINU},
        {"void",     sym::VOIDTK},
        {"*",        sym::MULT},
        {"/",        sym::DIV},
        {"%",        sym::MOD},
        {"<",        sym::LSS},
        {"<=",       sym::LEQ},
        {">",        sym::GRE},
        {">=",       sym::GEQ},
        {"==",       sym::EQL},
        {"!=",       sym::NEQ},
        {"=",        sym::ASSIGN},
        {";",        sym::SEMICN},
        {",",        sym::COMMA},
        {"(",        sym::LPARENT},
        {")",        sym::RPARENT},
        {"[",        sym::LBRACK},
        {"]",        sym::RBRACK},
        {"{",        sym::LBRACE},
        {"}",        sym::RBRACE},
        {"",         sym::NONETYPE}
    };
}

#endif //LEXICALANALYSIS_TABLES_H
