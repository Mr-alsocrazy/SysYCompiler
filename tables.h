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
