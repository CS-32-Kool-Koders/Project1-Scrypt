#include "./lib/lexer.h"
// #include "./lib/parser.h"
#include <iomanip>
#include <iostream>
// To run just the lexer, make sure you are in the src directory,
// then run: g++ -std=c++17 -Wall -Wextra -Werror lex.cpp ./lib/lexer.cpp
int main() {
    std::string line; // o store input
    int row = 0; 
    lexer Lexer;
    int new_line = 0; // 
    while(!std::cin.eof()) {
        //stores every instance of a new line
        new_line += 1;
        //goes through each line of input
        if(getline(std::cin, line)) { 
            row += 1;
            //makes tokens out of the line given
            //then puts them in tokenList
            Lexer.tokenize(row, line);
        }
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