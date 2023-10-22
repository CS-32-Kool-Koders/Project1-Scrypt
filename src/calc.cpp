#include <iostream>
#include "lib/infix_parser.h"
#include "lib/lexer.h"

// To run just the calc, make sure you are in the src directory,
// then run: g++ -std=c++17 -Wall -Wextra -Werror calc.cpp ./lib/infix_parser.cpp ./lib/lexer.cpp
// add -g for debugging with gdb/valgrind

// comment / uncomment this line to use dummy input from the assignment
// #define USE_DUMMY_INPUT

int main()
{
    std::vector<Tokens> tokens;

#ifndef USE_DUMMY_INPUT
    // CODE FROM LEX.CPP
    std::string line; // o store input
    int row = 0;
    lexer Lexer;
    int new_line = 0; //
    while (!std::cin.eof())
    {
        // stores every instance of a new line
        new_line += 1;
        // goes through each line of input
        if (getline(std::cin, line))
        {
            row += 1;
            // makes tokens out of the line given
            // then puts them in tokenList
            Lexer.tokenize(row, line);
        }
    }
    if (new_line > row)
    {
        Lexer.tokenList.push_back(Tokens(new_line, 1, "END"));
    }
    else
    {
        Lexer.tokenList.push_back(Tokens(row, Lexer.tokenList.back().col + 1, "END"));
    }
    // END OF CODE FROM LEX.CPP
    tokens = Lexer.tokenList;
#else
    tokens = {
        Tokens(1, 1, "x"), Tokens(1, 3, "="), Tokens(1, 5, "y"), Tokens(1, 7, "="),
        Tokens(1, 9, "0"), Tokens(1, 11, "+"), Tokens(1, 13, "1"), Tokens(1, 15, "+"),
        Tokens(1, 17, "2"), Tokens(1, 19, "*"), Tokens(1, 21, "3"), Tokens(1, 23, "-"),
        Tokens(1, 25, "4"), Tokens(1, 27, "/"), Tokens(1, 29, "("), Tokens(1, 30, "5"),
        Tokens(1, 32, "+"), Tokens(1, 34, "6"), Tokens(1, 35, ")")};
#endif

    // print out the tokens
    for (Tokens token : tokens)
    {
        std::cout << token.line << " " << token.col << " " << token.text << std::endl;
    }

    ExpressionParser parser(tokens);
    ExpressionNode *root = parser.parseExpression();
    root->printInfix();

    std::cout << std::endl;

    return 0;
}