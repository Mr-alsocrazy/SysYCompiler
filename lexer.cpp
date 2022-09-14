//
// Created by XYF on 2022/9/12.
//
#include "lexer.h"

void lexer::print_tk() {
    while (!is_end) {
        get_sym();
    }
    ifs.close();
    ofstream ofs;
    ofs.open("output.txt");
    auto it = token_vec.begin();
    for (; it < token_vec.end(); it++) {
        if (!(*it)->get_name().empty()) {
            switch ((*it)->get_symcode()) {
                case table::sym::IDENFER:
                    ofs << "IDENFER" << " " << (*it)->get_name() << endl;
                    break;
                case table::sym::INTCON:
                    ofs << "INTCON" << " " << (*it)->get_name() << endl;
                    break;
                case table::sym::STRCON:
                    ofs << "STRCON" << " " << (*it)->get_name() << endl;
                    break;
                default: {}
            }
        } else {
            switch ((*it)->get_symcode()) {
                case table::sym::PRINTFTK:
                    ofs << "PRINTFTK" << " " << "printf" << endl;
                    break;
                case table::sym::MAINTK:
                    ofs << "MAINTK" << " " << "main" << endl;
                    break;
                case table::sym::CONSTTK:
                    ofs << "CONSTTK" << " " << "const" << endl;
                    break;
                case table::sym::INTTK:
                    ofs << "INTTK" << " " << "int" << endl;
                    break;
                case table::sym::BREAKTK:
                    ofs << "BREAKTK" << " " << "break" << endl;
                    break;
                case table::sym::CONTINUETK:
                    ofs << "CONTINUETK" << " " << "continue" << endl;
                    break;
                case table::sym::IFTK:
                    ofs << "IFTK" << " " << "if" << endl;
                    break;
                case table::sym::ELSETK:
                    ofs << "ELSETK" << " " << "else" << endl;
                    break;
                case table::sym::NOT:
                    ofs << "NOT" << " " << "!" << endl;
                    break;
                case table::sym::AND:
                    ofs << "AND" << " " << "&&" << endl;
                    break;
                case table::sym::OR:
                    ofs << "OR" << " " << "or" << endl;
                    break;
                case table::sym::WHILETK:
                    ofs << "WHILETK" << " " << "while" << endl;
                    break;
                case table::sym::GETINTTK:
                    ofs << "GETINTTK" << " " << "getint" << endl;
                    break;
                case table::sym::RETURNTK:
                    ofs << "RETURNTK" << " " << "return" << endl;
                    break;
                case table::sym::PLUS:
                    ofs << "PLUS" << " " << "+" << endl;
                    break;
                case table::sym::MINU:
                    ofs << "MINU" << " " << "-" << endl;
                    break;
                case table::sym::VOIDTK:
                    ofs << "VOIDTK" << " " << "void" << endl;
                    break;
                case table::sym::MULT:
                    ofs << "MULT" << " " << "*" << endl;
                    break;
                case table::sym::DIV:
                    ofs << "DIV" << " " << "/" << endl;
                    break;
                case table::sym::MOD:
                    ofs << "MOD" << " " << "%" << endl;
                    break;
                case table::sym::LSS:
                    ofs << "LSS" << " " << "<" << endl;
                    break;
                case table::sym::LEQ:
                    ofs << "LEQ" << " " << "<=" << endl;
                    break;
                case table::sym::GRE:
                    ofs << "GRE" << " " << ">" << endl;
                    break;
                case table::sym::GEQ:
                    ofs << "GEQ" << " " << ">=" << endl;
                    break;
                case table::sym::EQL:
                    ofs << "EQL" << " " << "==" << endl;
                    break;
                case table::sym::NEQ:
                    ofs << "NEQ" << " " << "!=" << endl;
                    break;
                case table::sym::ASSIGN:
                    ofs << "ASSIGN" << " " << "=" << endl;
                    break;
                case table::sym::SEMICN:
                    ofs << "SEMICN" << " " << ";" << endl;
                    break;
                case table::sym::COMMA:
                    ofs << "COMMA" << " " << "," << endl;
                    break;
                case table::sym::LPARENT:
                    ofs << "LPARENT" << " " << "(" << endl;
                    break;
                case table::sym::RPARENT:
                    ofs << "RPARENT" << " " << ")" << endl;
                    break;
                case table::sym::LBRACK:
                    ofs << "LBRACK" << " " << "[" << endl;
                    break;
                case table::sym::RBRACK:
                    ofs << "RBRACK" << " " << "]" << endl;
                    break;
                case table::sym::LBRACE:
                    ofs << "LBRACE" << " " << "{" << endl;
                    break;
                case table::sym::RBRACE:
                    ofs << "RBRACE" << " " << "}" << endl;
                    break;
                case table::sym::NONETYPE:
                    break;
                default: {}
            }
        }
    }
}

inline void lexer::next_char() {
    if (cpos < line_length) {
        if (cpos == 0)
            l_id++;
        cur_char = cur_line[cpos++];
    }
    else if (ifs.eof()) {
        is_end = true;
    } else {
        char buf[1024];
        ifs.getline(buf, 1024);
        cur_line = string(buf);
        cur_char = ' ';
        cpos = 0;
        line_length = cur_line.size();
    }
}

inline void lexer::roll_back() {
    if (cpos > 0)
        cur_char = cur_line[--cpos];
}

pair<bool, table::sym> lexer::is_reserved(std::string &str) {
    auto kv = table::reserved.find(str);
    if (kv == table::reserved.end()) {
        return make_pair(false, table::sym::NONETYPE);
    } else {
        return make_pair(true, kv->second);
    }
}

void lexer::get_sym() {
    string match;
    while ((isspace(cur_char) || cur_char == '\t') && !is_end)
        next_char();
    if (isalpha(cur_char)) {
        match += cur_char;
        next_char();
        while (isalnum(cur_char) || cur_char == '_') {
            match += cur_char;
            next_char();
        } // basic string match without checking reserved tokens
        auto rp = is_reserved(match); //check if it is reserved
        if (rp.first) {
            if (rp.second == table::sym::PRINTFTK) {
                record_format = true;
            }
            token_vec.push_back(new Token(rp.second, lpos));
        } else {
            token_vec.push_back(new Token(table::sym::IDENFER, match, lpos));
        }
    } else if (isdigit(cur_char)) {
        match += cur_char;
        next_char();
        while (isdigit(cur_char)) {
            match += cur_char;
            next_char();
        }
        token_vec.push_back(new Token(table::sym::INTCON, match, lpos));
    } else if (cur_char == '(') {
        next_char();
        token_vec.push_back(new Token(table::sym::LPARENT, lpos));
    } else if (cur_char == ')') {
        next_char();
        token_vec.push_back(new Token(table::sym::RPARENT, lpos));
    } else if (cur_char == '[') {
        next_char();
        token_vec.push_back(new Token(table::sym::LBRACK, lpos));
    } else if (cur_char == ']') {
        next_char();
        token_vec.push_back(new Token(table::sym::RBRACK, lpos));
    } else if (cur_char == '{') {
        next_char();
        token_vec.push_back(new Token(table::sym::LBRACE, lpos));
    } else if (cur_char == '}') {
        next_char();
        token_vec.push_back(new Token(table::sym::RBRACE, lpos));
    } else if (cur_char == '!') {
        next_char();
        if (cur_char == '=') {
            token_vec.push_back(new Token(table::sym::NEQ, lpos));
        } else {
            roll_back();
            token_vec.push_back(new Token(table::sym::NOT, lpos));
        }
    } else if (cur_char == '|') {
        next_char();
        if (cur_char == '|') {
            token_vec.push_back(new Token(table::sym::OR, lpos));
        }
    } else if (cur_char == '&') {
        next_char();
        if (cur_char == '&') {
            token_vec.push_back(new Token(table::sym::AND, lpos));
        }
    } else if (cur_char == '+') {
        next_char();
        token_vec.push_back(new Token(table::sym::PLUS, lpos));
    } else if (cur_char == '-') {
        next_char();
        token_vec.push_back(new Token(table::sym::MINU, lpos));
    } else if (cur_char == '*') {
        next_char();
        roll_back();
        token_vec.push_back(new Token(table::sym::MULT, lpos));
    } else if (cur_char == '/') {
        next_char();
        token_vec.push_back(new Token(table::sym::DIV, lpos));
    } else if (cur_char == '%') {
        next_char();
        token_vec.push_back(new Token(table::sym::MOD, lpos));
    } else if (cur_char == '>') {
        next_char();
        if (cur_char == '=') {
            token_vec.push_back(new Token(table::sym::GEQ, lpos));
        } else {
            roll_back();
            token_vec.push_back(new Token(table::sym::GRE, lpos));
        }
    } else if (cur_char == '<') {
        next_char();
        if (cur_char == '=') {
            token_vec.push_back(new Token(table::sym::LEQ, lpos));
        } else {
            roll_back();
            token_vec.push_back(new Token(table::sym::LSS, lpos));
        }
    } else if (cur_char == '=') {
        next_char();
        if (cur_char == '=') {
            token_vec.push_back(new Token(table::sym::EQL, lpos));
        } else {
            roll_back();
            token_vec.push_back(new Token(table::sym::ASSIGN, lpos));
        }
    } else if (cur_char == ',') {
        next_char();
        token_vec.push_back(new Token(table::sym::COMMA, lpos));
    } else if (cur_char == ';') {
        next_char();
        token_vec.push_back(new Token(table::sym::SEMICN, lpos));
    } else if (cur_char == '\"' && record_format) {
        match += cur_char;
        next_char();
        while (cur_char != '\"') {
            match += cur_char;
            next_char();
        }
        match += cur_char;
        next_char();
        record_format = false;
        token_vec.push_back(new Token(table::sym::STRCON, match, lpos));
    }
}