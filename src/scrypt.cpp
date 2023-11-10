#include <iostream>
// #include "lib/infix_parser.h"
#include "lib/lexer.h"
#include "lib/blocks.h"
#include <string>
#include <sstream>
#include <algorithm>


void checkTokenString(const std::string &tokenString);
Blocks* parseStatements(std::vector<Tokens> tokens);

int main() {
    std::string line; 
    int row = 0; 
    lexer Lexer;
    int new_line = 0; 
    try{
        while(!std::cin.eof()) {
            new_line += 1;
            if(getline(std::cin, line)) { 
                row += 1;
                Lexer.tokenize(row, line);
            }
        }
    } 
    catch(const std::runtime_error& e){
        std::cout << e.what() << std::endl;
        exit(1);
    }
    if(new_line > row) {
        Lexer.tokenList.push_back(Tokens(new_line, 1, "END"));
    }
    else {
        Lexer.tokenList.push_back(Tokens(row, Lexer.tokenList.back().col+1, "END"));
    }
    

    //go through token list
    //add to some string, if end, then 
    
    return 0;
}

// bool checkoperator(std::string op){
//     std::vector<std::string> ops = {
//     "+", "-", "*", "/", "=", "END", "%",
//     "==", ">", ">=", "<", "<=", "|", "^", "&", "!=",
//     "+", "-", "*", "/", "=", "END", "%", "==", ">", 
//     ">=", "<", "<=", "|", "^", "&", "!=",
//     "+", "-", "*", "/", "=", "%", "==", ">", ">=",
//      "<", "<=", "|", "^", "&", "!="};
//     if (std::find(ops.begin(), ops.end(), op) == ops.end()) {
//         return false;
//     }
//     return true;
// }

// Blocks* parseStatements(std::vector<Tokens> tokens){
//     if (tokens.at(i))
// }
