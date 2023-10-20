#include <iostream>
#include "lib/infix_parser.h"

// To run just the calc, make sure you are in the src directory,
// then run: g++ -std=c++17 -Wall -Wextra -Werror calc.cpp ./lib/infix_parser.cpp

int main()
{
    std::vector<Tokens> tokens = {
        Tokens(1, 1, "x"), Tokens(1, 3, "="), Tokens(1, 5, "y"), Tokens(1, 7, "="),
        Tokens(1, 9, "0"), Tokens(1, 11, "+"), Tokens(1, 13, "1"), Tokens(1, 15, "+"),
        Tokens(1, 17, "2"), Tokens(1, 19, "*"), Tokens(1, 21, "3"), Tokens(1, 23, "-"),
        Tokens(1, 25, "4"), Tokens(1, 27, "/"), Tokens(1, 29, "("), Tokens(1, 30, "5"),
        Tokens(1, 32, "+"), Tokens(1, 34, "6"), Tokens(1, 35, ")")};

    ExpressionParser parser(tokens);
    ExpressionNode *root = parser.parseExpression();
    root->printInfix();

    std::cout << std::endl;

    return 0;
}