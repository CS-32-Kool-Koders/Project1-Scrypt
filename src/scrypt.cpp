#include <iostream>
// #include "lib/infix_parser.h"
#include "lib/lexer.h"
#include "lib/blocks.h"
#include <string>
#include <sstream>

void checkTokenString(const std::string &tokenString);

int main() {
    std::vector<Tokens> tokens;

    int new_line = 0;

    while (!std::cin.eof()) {
        new_line += 1;
        while (std::getline(std::cin, ExpressionParser::line)) {
            auto varSave = ExpressionParser::knowsVariables;
            auto varSave2 = ExpressionParser::variables;
            ExpressionNode *root = nullptr;

            try {
                lexer lexer;
                std::istringstream stream(ExpressionParser::line);
                lexer.tokenize(1, stream.str());
                lexer.tokenList.push_back(Tokens(1, lexer.tokenList.back().col + 1, "END"));

                std::string str = "";
                size_t temp = 0;

                for (size_t i = 0; i < lexer.tokenList.size(); i++) {
                    while (temp <= ExpressionParser::line.length() &&
                           ExpressionParser::line.substr(temp, temp + lexer.tokenList[i].text.length() - 1) != lexer.tokenList[i].text) {
                        str += ExpressionParser::line[temp];
                        temp++;
                    }
                    if (lexer.tokenList[i].text == "END") {
                        str += "END";
                    }
                }
                ExpressionParser::line += "END";
                //add code here to account for statements
                bool statement = false;
                size_t idx = 0;
                for (size_t i = 0; i < lexer.tokenList.size(); i++){
                    if (lexer.tokenList[i].text == "if" || lexer.tokenList[i].text == "while" 
                    || lexer.tokenList[i].text == "else" || lexer.tokenList[i].text == "print"){
                        statement = true;
                        idx = i;
                    }
                }
                if (statement){
                    std::vector<Tokens> blocklist;
                    // blocks.execute(lexer.tokenList);
                    // std::cout<<"detected statement";
                    idx ++;
                    while (lexer.tokenList[idx].text != "{"){
                        blocklist.push_back(lexer.tokenList[idx]);
                        idx++;
                    }
                    // for (size_t i = 0; i < blocklist.size(); i ++){
                    //     std::cout<<blocklist.at(i).text;
                    // }
                    //after getting the expression, keep lexing, parsing lines depending on condition
                    //maybe call execute method here?
                    ExpressionParser parser(blocklist);
                    Blocks blocks(lexer.tokenList.at(idx).text, blocklist, &parser);
                    //while loop here 
                    //maybe while input and while open (maintain boolean for open), keep reading in lines
                    //Lex each line, add each lexed line  go through each token list, if there is a }, let boolean be false and stop,
                    //
                    continue;
                }
    

                ExpressionParser parser(lexer.tokenList);
                root = parser.parseExpression();
                std::string tokenString = str;
                root->checkParentheses(tokenString);

                if (root != nullptr) {
                    root->getVariablesNames();
                    root->computeInfix();
                    BooleanWrapper resultVar = root->computeResult();
                    // root->printInfix();
                    root->printResult(resultVar);
                    delete root;
                } else {
                    checkTokenString(tokenString);
                }
            } catch (std::runtime_error &e) {
                if (root != nullptr) {
                    root->printTree();
                    std::cout << e.what() << std::endl;
                    delete root;
                    ExpressionParser::knowsVariables = varSave;
                    ExpressionParser::variables = varSave2;
                } else {
                    std::cout << e.what() << std::endl;
                    ExpressionParser::knowsVariables = varSave;
                    ExpressionParser::variables = varSave2;
                }
            } catch (std::logic_error &e) {
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

void checkTokenString(const std::string &tokenString) {
    for (size_t i = 0; i < tokenString.length(); i++) {
        if (tokenString[i] == '=') {
            if (tokenString[i + 1] == ')') {
                std::string throw_message = "Unexpected token at line 1 column " + std::to_string(i + 2) + ": " + tokenString[i + 1];
                throw std::logic_error(throw_message);
            } else {
                std::string throw_message = "Unexpected token at line 1 column " + std::to_string(i + 1) + ": " + tokenString[i];
                throw std::logic_error(throw_message);
            }
        }

        if (tokenString[i] == '+' || tokenString[i] == '-' || tokenString[i] == '/' || tokenString[i] == '*') {
            if (i == 0) {
                std::string throw_message = "Unexpected token at line 1 column " + std::to_string(i) + ": " + tokenString[i];
                throw std::logic_error(throw_message);
            } else if (i == tokenString.length() - 3) {
                std::string throw_message = "Unexpected token at line 1 column " + std::to_string(i + 1) + ": " + tokenString.substr(i);
                throw std::logic_error(throw_message);
            } else {
                if (tokenString[i] != '*') {
                    size_t temp = i;
                    i--;
                    while (std::isspace(tokenString[i])) {
                        i--;
                    }
                    if (!isdigit(tokenString[i])) {
                        std::string throw_message = "Unexpected token at line 1 column " + std::to_string(i + 1) + ": " + tokenString[i];
                        throw std::logic_error(throw_message);
                    }
                    i = temp;
                    i++;
                    while (std::isspace(tokenString[i])) {
                        i++;
                    }
                    if (i == temp) {
                        std::string throw_message = "Unexpected token at line 1 column " + std::to_string(i + 2) + ": " + tokenString[i + 1];
                        throw std::logic_error(throw_message);
                    } else if (!isdigit(tokenString[i]) && tokenString.substr(i) != "END") {
                        std::string throw_message = "Unexpected token at line 1 column " + std::to_string(i + 1) + ": " + tokenString[i];
                        throw std::logic_error(throw_message);
                    } else if (tokenString.substr(i) == "END") {
                        std::string throw_message = "Unexpected token at line 1 column " + std::to_string(i + 1) + ": " + tokenString.substr(i);
                        throw std::logic_error(throw_message);
                    }
                }
            }
        }
    }
}
