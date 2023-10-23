#include <iostream>
#include "lib/infix_parser.h"
#include "lib/lexer.h"
#include <string>
#include <sstream>

// To run just the calc, make sure you are in the src directory,
// then run: g++ -std=c++17 -Wall -Wextra -Werror calc.cpp ./lib/infix_parser.cpp ./lib/lexer.cpp
// add -g for debugging with gdb/valgrind

// comment / uncomment this line to use dummy input from the assignment
// #define USE_DUMMY_INPUT

int main()
{
    std::vector<Tokens> tokens;
    std::string line;
    int new_line = 0;

    while (!std::cin.eof())
    {
        new_line += 1;
        while (std::getline(std::cin, line))
        {
            try
            {
                lexer lexer;
                std::istringstream stream(line);
                lexer.tokenize(1, stream.str());
                lexer.tokenList.push_back(Tokens(1, lexer.tokenList.back().col + 1, "END"));

                ExpressionParser parser(lexer.tokenList);
                ExpressionNode *root = parser.parseExpression();
                root->printInfix();

                delete root;

                std::cout << std::endl;
            }
            catch (...)
            {
                std::cout << "error";
            }
        }
    }

    return 0;
}