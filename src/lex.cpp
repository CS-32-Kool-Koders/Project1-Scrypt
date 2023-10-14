#include <iostream>
#include <string>
#include "./lib/lexer.cpp"
#include "./lib/parser.h"
#include <iomanip>
//g++ -std=c++17 -Wall -Wextra -Werror lex.cpp
int main() {
    std::string line;
    int row = 0;
    lexer Lexer;
    int new_line = 0;
    //std::cout << "woot" <<std::endl;
    //goes through each line of input
    while(!std::cin.eof()) {
        //std::cout << "hahaha";
        new_line += 1;
        if(getline(std::cin, line)) { 
            row += 1;
            Lexer.tokenize(row, line);
        }
        // for(Tokens token : Lexer.tokenList) {
        //     std::cout << std::setw(4) << token.line << std::setw(5) << token.col << std::setw(2+token.text.length()) << token.text << std::endl;
        // }
    }
    if(new_line > row) {
        Lexer.tokenList.push_back(Tokens(new_line, 1, "END"));
    }
    else {
        Lexer.tokenList.push_back(Tokens(row, Lexer.tokenList.back().col+1, "END"));
    }
    for(Tokens token : Lexer.tokenList) {
        std::cout << std::setw(4) << token.line << std::setw(5) << token.col << std::setw(2+token.text.length()) << token.text << std::endl;
    }
    
    return 0;
}