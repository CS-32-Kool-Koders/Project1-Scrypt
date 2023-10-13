#include <iostream>
#include <string>
#include "./lib/lexer.cpp"
#include <iomanip>
//g++ -std=c++17 -Wall -Wextra -Werror lex.cpp
int main() {
    std::string line;
    int row = 0;
    lexer Lexer;
    //std::cout << "woot" <<std::endl;
    //goes through each line of input
    while(getline(std::cin, line)) {
        //std::cout << "hahaha";
        row += 1;
        Lexer.tokenize(row, line);
        for(Tokens token : Lexer.tokenList) {
            std::cout << std::setw(4) << token.line << std::setw(5) << token.col << std::setw(2+token.text.length()) << token.text << std::endl;
        }
    }
    if(std::cin.eof()) {
        Lexer.tokenList.push_back(Tokens(row+1, 1, "END"));
        for(Tokens token : Lexer.tokenList) {
            std::cout << std::setw(4) << token.line << std::setw(5) << token.col << std::setw(2+token.text.length()) << token.text << std::endl;
        }
    }
    return 0;
}