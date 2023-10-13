#include "lexer.h"
#include <ctype.h>

//Our constructor
//Goal: take user input (as a stream) & make an input string

//takes in lines and columns
// lexer::lexer(std::istream& in) : index(0), line(1), column(1)
//     input = std::string(std::istreambuf_iterator<char>(in), {})

//going char by char, add to the token
lexer::lexer() {
    //input = std::string(std::istreambuf_iterator<char>(in), {})
    

    //in lex.cpp in src, we'll have a while loop that goes thru input line-by-line
    //add goes thu each char
    //making tokens out of them
}

void lexer::tokenize(int row, std::string line) {
    //takes a line as input and 

    
    //call isDigit method
    //regarding tokenList.push_back(Tokens(row, (int)i, std::to_string(line[i])));
    //-> using to_string method on line[i], because 3rd arg is being read as a char
    //-> using (int) method on i,           because 2nd arg is being read as size(t) (which compiler doesn't like)
    for(size_t i=1; i <= line.size(); i++) {
        switch(line[i]){
            //case - plus
            case '+':
                tokenList.push_back(Tokens((int)row, (int)i, std::to_string(line[i])));
                break;
            //case - minus
            case '-':
                tokenList.push_back(Tokens((int)row, (int)i, std::to_string(line[i])));
                break;
            //case - multiply
            case '*':
                tokenList.push_back(Tokens((int)row, (int)i, std::to_string(line[i])));
                break;
            //case - divide
            case '/':
                tokenList.push_back(Tokens((int)row, (int)i, std::to_string(line[i])));
                break;
            //case - left_paren
            case '(':
                tokenList.push_back(Tokens((int)row, (int)i, std::to_string(line[i])));
                break;
            //case - right_paren
            case ')':
                tokenList.push_back(Tokens((int)row, (int)i, std::to_string(line[i])));
                break;
            case ' ':
                continue;
            default:
                checkIsDigit(line, (int)row, (int)i);
                //obtaining last item in tokenList
                i = tokenList.back().col;
                break;
        }
    }


    //gets token 
}

int lexer::checkIsDigit(std::string line, int row, int col) {
    int decimal = 0;
    //type string in case digit has a decimal
    std::string digit;
    if(isdigit(line[col])) {
        while(isdigit(line[col+1]) || line[col+1] == '.') {
            if(std::to_string(line[col+1]) == "." && decimal > 0) {
                    std::cout << "Syntax error on line " << row << " column " << col << ".";
                    return 1;
            }
            else if(line[col+1] == '.') {
                decimal++;
                digit += line[col+1];
                col++;
            }
            else {
                digit += line[col+1];
                col++;
            }
        }
    }
    else {
        std::cout << "Syntax error on line " << row << " column " << col << ".";
        return 1; 
    }

    tokenList.push_back(Tokens(row, col-1, digit));
    return 0;
}

