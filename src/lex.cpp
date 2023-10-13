#include <iostream>
#include <string>
#include "./lib/lexer.cpp"
#include <iomanip>
//g++ -std=c++17 -Wall -Wextra -Werror lex.cpp
int main() {
    std::string line;
    int row = 1;
    lexer Lexer;
    //std::cout << "woot" <<std::endl;
    //goes through each line of input
    while(getline(std::cin, line)) {
        //std::cout << "hahaha";
        Lexer.tokenize(row, line);
        row += 1;
        // for(Tokens token : Lexer.tokenList) {
        //     std::cout << std::setw(4) << token.line << std::setw(5) << token.col << std::setw(2+token.text.length()) << token.text << std::endl;
        // }
    }
    std::cout << "oo addding end";
    if(row > Lexer.tokenList.back().line) {
        Lexer.tokenList.push_back(Tokens(row, 1, "END"));
    }
    else {
        Lexer.tokenList.push_back(Tokens(row, Lexer.tokenList.back().col+1, "END"));
    }
    std::cout << "oo done adding end";
    for(Tokens token : Lexer.tokenList) {
        std::cout << std::setw(4) << token.line << std::setw(5) << token.col << std::setw(2+token.text.length()) << token.text << std::endl;
    }
    
    return 0;
}