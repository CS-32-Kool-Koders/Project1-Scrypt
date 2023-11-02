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
    int new_line = 0;

    while (!std::cin.eof())
    {
        new_line += 1;
        while (std::getline(std::cin, ExpressionParser::line))
        {
            auto varSave = ExpressionParser::knowsVariables;
            auto varSave2 = ExpressionParser::variables;
            ExpressionNode *root = nullptr;
            try
            {
                lexer lexer;
                std::istringstream stream(ExpressionParser::line);
                lexer.tokenize(1, stream.str());
                lexer.tokenList.push_back(Tokens(1, lexer.tokenList.back().col + 1, "END"));
                // std::cout << "line str: " << ExpressionParser::line << std::endl;

                std::string str = "";
                size_t temp = 0;
                for (size_t i = 0; i < lexer.tokenList.size(); i++)
                {
                    while (temp <= ExpressionParser::line.length() && ExpressionParser::line.substr(temp, temp + lexer.tokenList[i].text.length() - 1) != lexer.tokenList[i].text)
                    {
                        str += ExpressionParser::line[temp];
                        temp++;
                    }
                    if (lexer.tokenList[i].text == "END")
                    {
                        str += "END";
                    }
                }
                ExpressionParser::line += "END";
                // std::cout << "line str: " << ExpressionParser::line << std::endl;

                ExpressionParser parser(lexer.tokenList);
                root = parser.parseExpression();
                std::string tokenString = str;
                root->checkParentheses(tokenString);

                if (root != nullptr)
                {
                    root->getVariablesNames();
                    root->computeInfix();
                    root->printInfix();
                    root->printResult();
                    delete root;
                }
                else
                {
                    for (size_t i = 0; i < tokenString.length(); i++)
                    {
                        if (tokenString[i] == '=')
                        {
                            if (tokenString[i + 1] == ')')
                            {
                                std::string throw_message = "Unexpected token at line 1 column " + std::to_string(i + 2) + ": " + tokenString[i + 1];
                                throw std::logic_error(throw_message);
                            }
                            else
                            {
                                std::string throw_message = "Unexpected token at line 1 column " + std::to_string(i + 1) + ": " + tokenString[i];
                                throw std::logic_error(throw_message);
                            }
                        }

                        if (tokenString[i] == '+' || tokenString[i] == '-' || tokenString[i] == '/' || tokenString[i] == '*')
                        {
                            if (i == 0)
                            {
                                std::string throw_message = "Unexpected token at line 1 column " + std::to_string(i) + ": " + tokenString[i];
                                throw std::logic_error(throw_message);
                            }
                            else if (i == tokenString.length() - 3)
                            {
                                std::string throw_message = "Unexpected token at line 1 column " + std::to_string(i + 1) + ": " + tokenString.substr(i);
                                throw std::logic_error(throw_message);
                            }
                            else
                            {
                                if (tokenString[i] != '*')
                                {
                                    size_t temp = i;
                                    i--;
                                    while (std::isspace(tokenString[i]))
                                    {
                                        i--;
                                    }
                                    if (!isdigit(tokenString[i]))
                                    {
                                        std::string throw_message = "Unexpected token at line 1 column " + std::to_string(i + 1) + ": " + tokenString[i];
                                        throw std::logic_error(throw_message);
                                    }
                                    i = temp;
                                    i++;
                                    while (std::isspace(tokenString[i]))
                                    {
                                        i++;
                                    }
                                    if (i == temp)
                                    {
                                        std::string throw_message = "Unexpected token at line 1 column " + std::to_string(i + 2) + ": " + tokenString[i + 1];
                                        throw std::logic_error(throw_message);
                                    }
                                    else if (!isdigit(tokenString[i]) && tokenString.substr(i) != "END")
                                    {
                                        std::string throw_message = "Unexpected token at line 1 column " + std::to_string(i + 1) + ": " + tokenString[i];
                                        throw std::logic_error(throw_message);
                                    }
                                    else if (tokenString.substr(i) == "END")
                                    {
                                        std::string throw_message = "Unexpected token at line 1 column " + std::to_string(i + 1) + ": " + tokenString.substr(i);
                                        throw std::logic_error(throw_message);
                                    }
                                }
                            }
                        }
                    }
                    // std::cout << "root is null lolollol, root value: " << line << std::endl;

                    //     std::string throw_message = "Unexpected token at line 1 column " + std::to_string(column) + ": " + value;
                    //     throw std::logic_error(throw_message);
                }
            }
            catch (std::runtime_error &e)
            {
                if (root != nullptr)
                {
                    root->printTree();
                    std::cout << e.what() << std::endl;
                    delete root;
                    ExpressionParser::knowsVariables = varSave;
                    ExpressionParser::variables = varSave2;
                }
                else
                {
                    std::cout << e.what() << std::endl;
                    ExpressionParser::knowsVariables = varSave;
                    ExpressionParser::variables = varSave2;
                    // return 1;
                }
            }
            catch (std::logic_error &e)
            {
                std::cout << e.what() << std::endl;
                if (root != nullptr)
                    delete root;
                ExpressionParser::knowsVariables = varSave;
                ExpressionParser::variables = varSave2;
            }
        }
    }
    return 0;
}
