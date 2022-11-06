//
// Created by XYF on 2022/9/12.
//
#include "lexer.h"

#include <memory>

vector<shared_ptr<Token>> token_vec;

void lexer::lexing() {
    while (!is_end) {
        get_sym();
    }
}

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
                case table::sym::IDENFR:
                    ofs << "IDENFR" << " " << (*it)->get_name() << endl;
                    break;
                case table::sym::INTCON:
                    ofs << "INTCON" << " " << (*it)->get_name() << endl;
                    break;
                case table::sym::STRCON:
                    ofs << "STRCON" << " " << (*it)->get_name() << endl;
                    break;
                default: {
                }
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
                    ofs << "OR" << " " << "||" << endl;
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
                default: {
                }
            }
        }
    }
    ofs.close();
}

void lexer::next_char() {
    l_id = lpos;
    if (cpos < line_length) {
        if (cpos == 0)
            l_id++;
        cur_char = cur_line[cpos++];
    } else if (ifs.eof()) {
        is_end = true;
    } else {
        getline(ifs, cur_line);
        lpos++;
        while (cur_line.empty() && !ifs.eof()) {
            getline(ifs, cur_line);
            lpos++;
        }
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
    if (isalpha(cur_char) || cur_char == '_') {
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
            token_vec.push_back(std::make_shared<Token>(rp.second, l_id));
        } else {
            token_vec.push_back(std::make_shared<Token>(table::sym::IDENFR, match, l_id));
        }
    } else if (isdigit(cur_char)) {
        match += cur_char;
        next_char();
        while (isdigit(cur_char)) {
            match += cur_char;
            next_char();
        }
        token_vec.push_back(std::make_shared<Token>(table::sym::INTCON, stoi(match), l_id));
    } else if (cur_char == '(') {
        next_char();
        token_vec.push_back(std::make_shared<Token>(table::sym::LPARENT, l_id));
    } else if (cur_char == ')') {
        next_char();
        token_vec.push_back(std::make_shared<Token>(table::sym::RPARENT, l_id));
    } else if (cur_char == '[') {
        next_char();
        token_vec.push_back(std::make_shared<Token>(table::sym::LBRACK, l_id));
    } else if (cur_char == ']') {
        next_char();
        token_vec.push_back(std::make_shared<Token>(table::sym::RBRACK, l_id));
    } else if (cur_char == '{') {
        next_char();
        token_vec.push_back(std::make_shared<Token>(table::sym::LBRACE, l_id));
    } else if (cur_char == '}') {
        next_char();
        token_vec.push_back(std::make_shared<Token>(table::sym::RBRACE, l_id));
    } else if (cur_char == '!') {
        next_char();
        if (cur_char == '=') {
            next_char();
            token_vec.push_back(std::make_shared<Token>(table::sym::NEQ, l_id));
        } else {
            token_vec.push_back(std::make_shared<Token>(table::sym::NOT, l_id));
        }
    } else if (cur_char == '|') {
        next_char();
        if (cur_char == '|') {
            next_char();
            token_vec.push_back(std::make_shared<Token>(table::sym::OR, l_id));
        }
    } else if (cur_char == '&') {
        next_char();
        if (cur_char == '&') {
            next_char();
            token_vec.push_back(std::make_shared<Token>(table::sym::AND, l_id));
        }
    } else if (cur_char == '+') {
        next_char();
        token_vec.push_back(std::make_shared<Token>(table::sym::PLUS, l_id));
    } else if (cur_char == '-') {
        next_char();
        token_vec.push_back(std::make_shared<Token>(table::sym::MINU, l_id));
    } else if (cur_char == '*') {
        next_char();
        token_vec.push_back(std::make_shared<Token>(table::sym::MULT, l_id));
    } else if (cur_char == '/') {
        next_char();
        if (cur_char == '/') {
            int fls = cpos;
            int tmp_l = line_length;
            for (int i = 0; i < tmp_l - fls + 1; i++)
                next_char();
        } else if (cur_char == '*') {
            while (!is_end) {
                next_char();
                if (cur_char == '*') {
                    next_char();
                    if (cur_char == '/') {
                        next_char();
                        break;
                    }
                    roll_back();
                }
            }
        } else {
            token_vec.push_back(std::make_shared<Token>(table::sym::DIV, l_id));
        }
    } else if (cur_char == '%') {
        next_char();
        token_vec.push_back(std::make_shared<Token>(table::sym::MOD, l_id));
    } else if (cur_char == '>') {
        next_char();
        if (cur_char == '=') {
            next_char();
            token_vec.push_back(std::make_shared<Token>(table::sym::GEQ, l_id));
        } else {
            token_vec.push_back(std::make_shared<Token>(table::sym::GRE, l_id));
        }
    } else if (cur_char == '<') {
        next_char();
        if (cur_char == '=') {
            next_char();
            token_vec.push_back(std::make_shared<Token>(table::sym::LEQ, l_id));
        } else {
            token_vec.push_back(std::make_shared<Token>(table::sym::LSS, l_id));
        }
    } else if (cur_char == '=') {
        next_char();
        if (cur_char == '=') {
            next_char();
            token_vec.push_back(std::make_shared<Token>(table::sym::EQL, l_id));
        } else {
            token_vec.push_back(std::make_shared<Token>(table::sym::ASSIGN, l_id));
        }
    } else if (cur_char == ',') {
        next_char();
        token_vec.push_back(std::make_shared<Token>(table::sym::COMMA, l_id));
    } else if (cur_char == ';') {
        next_char();
        token_vec.push_back(std::make_shared<Token>(table::sym::SEMICN, l_id));
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
        token_vec.push_back(std::make_shared<Token>(table::sym::STRCON, match, l_id));
    }
}