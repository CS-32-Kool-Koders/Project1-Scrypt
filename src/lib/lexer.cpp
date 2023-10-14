#include "lexer.h"
#include <ctype.h>
#include <string>

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

int lexer::tokenize(int row, std::string line) {
    //takes a line as input and 
    
    //call isDigit method
    //regarding tokenList.push_back(Tokens(row, (int)i, std::to_string(line[i])));
    //-> using to_string method on line[i], because 3rd arg is being read as a char
    //-> using (int) method on i,           because 2nd arg is being read as size(t) (which compiler doesn't like)
    for(size_t i=0; i < line.size(); i++) {
        //case - plus
        if(line[i] == '+') {
            tokenList.push_back(Tokens((int)row, (int)i+1, std::string(1, line[i])));
            //break;
        }
        //case - minus
        else if(line[i] == '-') {
            tokenList.push_back(Tokens((int)row, (int)i+1, std::string(1, line[i])));
            //break;
        }
        //case - multiply
        else if(line[i] == '*'){
            tokenList.push_back(Tokens((int)row, (int)i+1, std::string(1, line[i])));
            //break;
        }
        //case - divide
        else if(line[i] == '/'){
            tokenList.push_back(Tokens((int)row, (int)i+1, std::string(1, line[i])));
            //break;
        }
        //case - left_paren
        else if(line[i] == '('){
            tokenList.push_back(Tokens((int)row, (int)i+1, std::string(1, line[i])));
            //break;
        }
        //case - right_paren
        else if(line[i] == ')'){
            tokenList.push_back(Tokens((int)row, (int)i+1, std::string(1, line[i])));
            //break;
        }
        else if(isspace(line[i])){
            //std::cout << "space";
            //break;
        }
        else if (isdigit(line[i]) || line[i] == '.') {
            checkIsDigit(line, (int)row, (int)i);
            //i = i+1;
            //obtaining last item in tokenList
            i = tokenList.back().col+tokenList.back().text.size()-2;
            //break;
        }
        else {
            std::cout << "Syntax error on line " << row << " column " << i+1 << "." << std::endl;
            exit(1);
        }
    }

    return 0;
    //gets token 
}

int lexer::checkIsDigit(std::string line, int row, int col) {
    int decimal = 0;
    int offset = 0;
    //type string in case digit has a decimal
    std::string digit;
    if(isdigit(line[col])) {
        digit += line[col];
        while(isdigit(line[col+1]) || line[col+1] == '.') {
            //std::string next_line = std::string(1, line[col+1]);
            if(line[col+1] == '.' && decimal > 0) {
                    std::cout << "Syntax error on line " << row << " column " << col+2 << "." << std::endl;
                    exit(1);
            }
            else if(line[col+1] == '.') {
                decimal++;
                digit += line[col+1];
                col++;
                offset +=1;
            }
            else {
                digit += line[col+1];
                col++;
                offset +=1;
            }
        }
    }
    else {
        std::cout << "Syntax error on line " << row << " column " << col+2 << "." << std::endl;
        exit(1); 
    }

    tokenList.push_back(Tokens(row, col-offset+1, digit));
    return 0;
}

