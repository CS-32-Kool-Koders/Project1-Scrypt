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
            auto varSave = ExpressionParser::knowsVariables;
            auto varSave2 = ExpressionParser::variables;
            ExpressionNode *root = nullptr;
            try
            {
                lexer lexer;
                std::istringstream stream(line);
                lexer.tokenize(1, stream.str());
                lexer.tokenList.push_back(Tokens(1, lexer.tokenList.back().col + 1, "END"));

                std::string str = "";
                size_t temp = 0;
                for (size_t i = 0; i < lexer.tokenList.size(); i++)
                {
                    while (temp <= line.length() && line.substr(temp, temp + lexer.tokenList[i].text.length() - 1) != lexer.tokenList[i].text)
                    {
                        str += line[temp];
                        temp++;
                    }
                    if (lexer.tokenList[i].text == "END")
                    {
                        str += "END";
                    }
                }
                int paren_count = 0;
                std::string tokenString = str;

                // std::cout << "tokenString: " << tokenString << std::endl;
                for (size_t i = 0; i < tokenString.length(); i++)
                {
                    // std::cout << "Token at " << i << " " << tokenlist[i] << std::endl;
                    if (tokenString[i] == '(')
                    {
                        paren_count++;
                        // Checks if parentheses set is empty
                        size_t temp = i;
                        size_t space_count = 0;
                        while (tokenString[temp] != ')' && temp < tokenString.length())
                        {
                            if (std::isspace(tokenString[temp]))
                            {
                                space_count++;
                            }
                            temp++;
                        }
                        if (space_count == temp - i - 1)
                        {
                            std::string throw_message = "Unexpected token at line 1 column " + std::to_string(temp + 1) + ": " + tokenString[temp];
                            throw std::logic_error(throw_message);
                        }
                    }
                    else if (tokenString[i] == ')')
                    {
                        paren_count--;
                    }
                    int int_listSize = tokenString.length();
                    // Checks if END is not where its supposed to be and if the parenthesis are equal
                    if (tokenString.substr(i, i + 2) == "END" && (i != (size_t)int_listSize - 3 || paren_count != 0))
                    {
                        std::string throw_message = "Unexpected token at line 1 column " + std::to_string(i) + ": " + tokenString.substr(i, i + 2);
                        throw std::logic_error(throw_message);
                    }
                    // If it reaches "END" and "END" is where its supposed to be, and the parentheses are closed
                    else if (tokenString.substr(i, i + 2) == "END" && i + 2 == (size_t)int_listSize - 1 && paren_count == 0)
                    {
                        break;
                    }
                    // If there are more close parenthesis or there are too many open parentheses and not enough space left to close them
                    else if ((paren_count < 0) || paren_count > int_listSize - 1 - (int)i)
                    {
                        // std::cout << "Paren count " << paren_count <<std::endl;
                        // std::cout << "i " << i << std::endl;
                        // // //std::cout << "offset " << offset << std::endl;
                        // std::cout << "int_listSize " << int_listSize << std::endl;
                        std::string throw_message = "Unexpected token at line 1 column " + std::to_string(i + 1) + ": " + tokenString[i];
                        throw std::logic_error(throw_message);
                    }
                }

                ExpressionParser parser(lexer.tokenList);
                root = parser.parseExpression();
                // if(root == nullptr) {
                //     throw std::runtime_error("Unexpected token at line 1 column "+ +"");
                // }
                if (root != nullptr)
                {
                    // std::cout << "root is not null" << std::endl;
                    root->getVariablesNames();
                    root->printInfix();
                    // std::cout << "part 1" << std::endl;
                    double result = root->computeResult();
                    // std::cout << "part 2" << std::endl;
                    root->printResult();
                    // std::cout << "part 3" << std::endl;
                    std::cout << result << std::endl;
                }
                else
                {
                    for (size_t i = 0; i < tokenString.length(); i++)
                    {
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
                                else if (!isdigit(tokenString[i]))
                                {
                                    std::string throw_message = "Unexpected token at line 1 column " + std::to_string(i + 1) + ": " + tokenString[i];
                                    throw std::logic_error(throw_message);
                                }
                            }
                        }
                    }
                    // std::cout << "root is null lolollol, root value: " << line << std::endl;

                    //     std::string throw_message = "Unexpected token at line 1 column " + std::to_string(column) + ": " + value;
                    //     throw std::logic_error(throw_message);
                }

                delete root;
            }
            catch (std::runtime_error &e)
            {
                root->printTree();
                std::cout << e.what() << std::endl;
                if (root != nullptr)
                    delete root;
                ExpressionParser::knowsVariables = varSave;
                ExpressionParser::variables = varSave2;
                // return 1;
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
