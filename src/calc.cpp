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


                int paren_count = 0;
                //int offset = 0;
                std::string tokenlist = stream.str();
                for(size_t i =0; i < tokenlist.length(); i++) {
                    std::cout << "Token at " << i << " " << tokenlist[i] << std::endl;
                    if(tokenlist[i] == '(') {
                        paren_count++;
                    }
                    else if(tokenlist[i] == ')') {
                        paren_count--;
                    }
                    int int_listSize = tokenlist.length();
                    if(tokenlist.substr(i,i+2) == "END" && i != (size_t)int_listSize-1) {
                        //std::cout << "the end";
                        std::string throw_message = "Unexpected token at line 1 column " + std::to_string(i) + ": " + tokenlist.substr(i, i+2);
                        throw std::logic_error(throw_message);
                    }
                    else if((paren_count < 0) || (paren_count == 0 && (i < (size_t) int_listSize-2) && (i >0)) || paren_count > int_listSize-1-(int)i){
                        // std::cout << "Paren count " << paren_count <<std::endl;
                        // std::cout << "i " << i << std::endl;
                        // // //std::cout << "offset " << offset << std::endl;
                        // std::cout << "int_listSize " << int_listSize << std::endl;
                        std::string throw_message = "Unexpected token at line 1 column " + std::to_string(i) + ": " + tokenlist[i];
                        throw std::logic_error(throw_message);
                    }
                    // else {
                    //     std::string throw_message = "Unexpected token at line 1 column " + std::to_string(lexer.tokenList[i].col) + ": " + lexer.tokenList[i].text;
                    //     throw std::logic_error(throw_message);
                    // }
                }

                
                // if(paren_count != 0) {
                //     std::string throw_message = "Unexpected token at line 1 column " + std::to_string(lexer.tokenList.back().col) + ": " + lexer.tokenList.back().text;
                //     throw std::logic_error(throw_message);
                // }

                ExpressionParser parser(lexer.tokenList);
                root = parser.parseExpression();
                // if(root == nullptr) {
                //     throw std::runtime_error("Unexpected token at line 1 column "+ +"");
                // }
                root->getVariablesNames();
                root->printInfix();
                double result = root->computeResult();
                root->printResult();
                std::cout << result << std::endl;

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
