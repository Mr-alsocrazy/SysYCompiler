//
// Created by XYF on 2022/9/12.
//
#include "lexer.h"

void read_file() {
    ifstream ifs;
    ifs.open("testfile.txt");
    string buf;
    int idx = 0;
    while(getline(ifs, buf)) {
        lexer(buf, ++idx);
    }
}

void lexer::next_char() {
    if (cpos < line_length)
        cur_char = cur_line[cpos++];
    else
        is_end = true;
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
    while (!is_end) { // and now we build an automata
        match.clear();
        while (isspace(cur_char) && !is_end)
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
                token_vec.push_back(new Token(table::sym::NOT, lpos));
            }
        }
    }
}