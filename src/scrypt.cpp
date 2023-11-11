#include <iostream>
// #include "lib/infix_parser.h"
#include "lib/lexer.h"
#include "lib/blocks.h"
#include <string>
#include <sstream>
#include <algorithm>

bool isStatement(const std::string &tokenText)
{
    return tokenText == "if" || tokenText == "while" || tokenText == "else";
}
void checkTokenString(const std::string &tokenString);
// OVERHERE
Blocks *parseStatements(std::vector<std::vector<Tokens>> &lines, size_t &lineIndex, size_t &tokenIndex);

// Blocks* parseStatements(std::vector<Tokens>& tokens, const size_t& i=0);
Blocks *parseStatements(std::vector<std::vector<Tokens>> &lines, size_t &lineIndex, size_t &tokenIndex);

bool checkoperator(std::string op);
BooleanWrapper evaluateExpression(ExpressionParser *parser);
void evaluateBlock(Blocks *block);

void checkTokenString(const std::string &tokenString)
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
}

// void recDebugPrint(Blocks block) {
//     recursiveDebuggingPrint(block);
// }


int main()
{
    std::string line;
    int row = 0;
    lexer Lexer;
    int new_line = 0;
    try
    {
        while (!std::cin.eof())
        {
            new_line += 1;
            if (getline(std::cin, line))
            {
                row += 1;
                Lexer.tokenize(row, line);
            }
        }
    }
    catch (const std::runtime_error &e)
    {
        std::cout << e.what() << std::endl;
        exit(1);
    }
    if (new_line > row)
    {
        Lexer.tokenList.push_back(Tokens(new_line, 1, "END"));
    }
    else
    {
        Lexer.tokenList.push_back(Tokens(row, Lexer.tokenList.back().col + 1, "END"));
    }

    // go through token list
    // add to some string, if end, then
    //  std::cout << "tokenlist size = " << Lexer.tokenList.size() << std::endl;
    //  std::cout << "tokenlist size = " << Lexer.tokenList.size() << std::endl;
    std::vector<std::vector<Tokens>> tokensByLine;
    std::vector<Tokens> temp;
    size_t index = 0;
    while (index + 1 < Lexer.tokenList.size())
    {
        // std::cout << "index is " << index << std::endl;
        // std::cout << "token at index is " << Lexer.tokenList.at(index).text << std::endl;
        if (Lexer.tokenList.at(index).text == "if" || Lexer.tokenList.at(index).text == "while" || Lexer.tokenList.at(index).text == "else")
        {
            while (Lexer.tokenList.at(index).text != "{")
            {
                temp.push_back(Lexer.tokenList.at(index));
                index++;
            }
            temp.push_back(Lexer.tokenList.at(index));
            tokensByLine.push_back(temp);
            temp.clear();
            index++;
        }
        else if (Lexer.tokenList.at(index).text == "print")
        {
            temp.push_back(Lexer.tokenList.at(index));
            index++;
        }
        else if (Lexer.tokenList.at(index).text == "}")
        {
            // std::cout << "is closing paren" << std::endl;
            temp.push_back(Lexer.tokenList.at(index));
            tokensByLine.push_back(temp);
            temp.clear();
            index++;
        }
        else if (!checkoperator(Lexer.tokenList.at(index).text) && !checkoperator(Lexer.tokenList.at(index + 1).text))
        {
            // std::cout << "is not operator AND next is not operator" << std::endl;
            if (Lexer.tokenList.at(index + 1).text == "}")
            {
                // std::cout << "BUT NEXT WAS PAREN" << std::endl;
                temp.push_back(Lexer.tokenList.at(index));
                temp.push_back(Lexer.tokenList.at(index + 1));
                tokensByLine.push_back(temp);
                temp.clear();
                index += 2;
            }
            else
            {
                temp.push_back(Lexer.tokenList.at(index));
                tokensByLine.push_back(temp);
                temp.clear();
                // temp.push_back(Lexer.tokenList.at(index + 1));
                index++;
            }
        }
        else if (!checkoperator(Lexer.tokenList.at(index).text) && checkoperator(Lexer.tokenList.at(index + 1).text))
        {
            // std::cout << "is not operator AND next is operator";
            temp.push_back(Lexer.tokenList.at(index));
            temp.push_back(Lexer.tokenList.at(index + 1));
            index += 2;
        }
    }

    std::vector<Blocks *> astNodes;
    size_t lineIndex = 0, tokenIndex = 0;
    while (lineIndex < tokensByLine.size())
    {
        Blocks *astNode = nullptr;
        astNode = parseStatements(tokensByLine, lineIndex, tokenIndex);
        // if (astNode) {
        astNodes.push_back(astNode);
        // lineIndex++;
        // }
    }

    // for (Blocks *rootBlock : astNodes)
    // {
    //     std::string str = "";
    //     size_t temp = 0;
    //     for (size_t i = 0; i < Lexer.tokenList.size(); i++)
    //     {
    //         while (temp <= ExpressionParser::line.length() && ExpressionParser::line.substr(temp, temp + Lexer.tokenList[i].text.length() - 1) != Lexer.tokenList[i].text)
    //         {
    //             str += ExpressionParser::line[temp];
    //             temp++;
    //         }
    //         if (Lexer.tokenList[i].text == "END")
    //         {
    //             str += "END";
    //         }
    //     }
    //     // std::cout<<"This is a block"<<std::endl;
    //     // std::cout << "the type IS HERE: " << rootBlock->type << std::endl;
    //     // for (Blocks *block : rootBlock->blocklist)
    //     // {
    //     //     std::cout << "heres the type: " << block->type << std::endl;
    //     //     std::cout << "ITS THE CONDITION: ";
    //     //     for (Tokens token : block->condition->getTokens())
    //     //     {
    //     //         std::cout << token.text << " ";
    //     //     }
    //     //     std::cout << std::endl;
    //     // }
    //     //rootBlock->recursiveDebuggingPrint();
    //     // rootBlock->printInfo();
    //     ExpressionNode *root = rootBlock->condition->parseExpression();
    //     std::string tokenString = str;
    //     root->checkParentheses(tokenString);

    //     if (root != nullptr)
    //     {
    //         root->getVariablesNames();
    //         root->computeInfix();
    //         // BooleanWrapper resultVar = root->computeResult();
    //         // root->printInfix();
    //         // root->printResult(resultVar);
    //         delete root;
    //     }
    //     else
    //     {
    //         checkTokenString(tokenString);
    //     }
    //     // std::cout << root->value << std::endl;
    // }
    // for (Blocks *rootBlock : astNodes)
    // {
    //     std::cout << rootBlock->type << std::endl;
    // }
    for (Blocks *rootBlock : astNodes)
    {
        evaluateBlock(rootBlock);
    }

    // Clean-up (if necessary)
    // for (Blocks* node : astNodes) {
    //     delete node;  // Assuming dynamic allocation in parseStatements and parseBlock
    // }
    return 0;
}

bool checkoperator(std::string op)
{
    std::vector<std::string> ops = {
        "+", "-", "*", "/", "=", "%",
        "==", ">", ">=", "<", "<=", "|", "^", "&", "!=",
        "+", "-", "*", "/", "=", "%", "==", ">",
        ">=", "<", "<=", "|", "^", "&", "!=",
        "+", "-", "*", "/", "=", "%", "==", ">", ">=",
        "<", "<=", "|", "^", "&", "!="};
    if (std::find(ops.begin(), ops.end(), op) == ops.end())
    {
        return false;
    }
    return true;
}

Blocks *parseStatements(std::vector<std::vector<Tokens>> &lines, size_t &lineIndex, size_t &tokenIndex)
{
    if (lineIndex >= lines.size())
        return nullptr;

    std::vector<Tokens> &tokens = lines[lineIndex];
    Blocks *block = new Blocks();

    if (tokens.front().text == "if" || tokens.front().text == "while")
    {
        block->type = tokens.front().text;
        std::vector<Tokens> conditionTokens(tokens.begin() + 1, tokens.end() - 1); // Exclude 'if'/'while' and '{'
        block->condition = new ExpressionParser(conditionTokens);

        lineIndex++;
        tokenIndex = 0;
        // thenBlock
        // OVERHERE
        // for (std::vector<Tokens> vec : lines)
        // {
        //     for (Tokens token : vec)
        //     {
        //         std::cout << token.text;
        //     }
        //     std::cout << std::endl;
        // }
        while (lineIndex < lines.size() && lines[lineIndex].back().text != "}")
        {
            // std::cout << "This is the thing " << lines[lineIndex ].back().text << std::endl;
            block->blocklist.push_back(parseStatements(lines, lineIndex, tokenIndex));
            // lineIndex++;
        }
        block->blocklist.push_back(parseStatements(lines, lineIndex, tokenIndex));
        // block = parseStatements(lines, lineIndex, tokenIndex);

        // Handle 'else' or 'else if'
        if (lineIndex < lines.size() && lines[lineIndex].front().text == "else")
        {
            lineIndex++;
            tokenIndex = 0; // HMMMMMMMMM
            // Blocks *block = new Blocks();
            if (lineIndex < lines.size() && lines[lineIndex].front().text == "if")
            {
                // This is an 'else if' block
                // elseBlock
                block->blocklist.push_back(parseStatements(lines, lineIndex, tokenIndex));
                // block = parseStatements(lines, lineIndex, tokenIndex);
            }
            else
            {
                // This is an 'else' block
                tokenIndex = 0;
                // elseBlock
                // OVERHERE
                block->blocklist.push_back(parseStatements(lines, lineIndex, tokenIndex));
                // block = parseStatements(lines, lineIndex, tokenIndex);
            }
        }
    }
    else if (tokens.front().text == "else")
    {
        block->type = "else";
        lineIndex++;
        // if (lineIndex >= lines.size())
        // {
        //     return block;
        // }
        while (lineIndex < lines.size() && lines[lineIndex].back().text != "}")
        {
            // std::cout << "This is the thing " << lines[lineIndex ].back().text << std::endl;
            block->blocklist.push_back(parseStatements(lines, lineIndex, tokenIndex));
            // lineIndex++;
        }
        block->blocklist.push_back(parseStatements(lines, lineIndex, tokenIndex));
    }
    else if (tokens.front().text == "print")
    {
        block->type = "print";
        std::vector<Tokens> printExpressionTokens(tokens.begin() + 1, tokens.end());
        block->condition = new ExpressionParser(printExpressionTokens);
        lineIndex++;
        return block;
    }
    else
    {
        block->type = "expression";
        block->condition = new ExpressionParser(tokens);
        lineIndex++;
        return block;
    }

    lineIndex++;
    return block;
}

// OVERHERE
/*Blocks *parseBlock(std::vector<std::vector<Tokens>> &lines, size_t &lineIndex, size_t &tokenIndex)
{
    Blocks *block = new Blocks();
    block->type = "block";

    while (lineIndex < lines.size() && lines[lineIndex][0].text != "}")
    {
        Blocks *statement = parseStatements(lines, lineIndex, tokenIndex);
        if (statement)
        {
            block->blocklist.push_back(statement);
        }
    }

    if (lineIndex < lines.size() && lines[lineIndex][0].text == "}")
    {
        lineIndex++; // Move to the next line after the block
        tokenIndex = 0;
    }

    return block;
}*/

void evaluateBlock(Blocks *block) {
    if (!block) return;

    // Evaluate conditions for 'if' and 'while' blocks
    BooleanWrapper resultVar;
    if (block->type == "if" || block->type == "while") {
        ExpressionNode *root = block->condition->parseExpression();
        if (root != nullptr) {
            root->getVariablesNames();
            root->computeInfix();
            resultVar = root->computeResult();
            delete root;
        }
    }

    // Handle 'if' blocks
    if (block->type == "if") {
        if (resultVar.getBvalue()) {
            // Evaluate the first block in blocklist (thenBlock)
            for (Blocks *innerBlock : block->blocklist) {
                evaluateBlock(innerBlock);
            }
        }
    } 
    // Handle 'while' blocks
    else if (block->type == "while") {
        while (resultVar.getBvalue()) {
            for (Blocks *innerBlock : block->blocklist) {
                evaluateBlock(innerBlock);
            }

            // Re-evaluate condition for the while loop
            ExpressionNode *root = block->condition->parseExpression();
            if (root != nullptr) {
                root->getVariablesNames();
                root->computeInfix();
                resultVar = root->computeResult();
                delete root;
            }
        }
    }
    // Handle 'else' blocks
    else if (block->type == "else") {
        for (Blocks *innerBlock : block->blocklist) {
            evaluateBlock(innerBlock);
        }
    }
    // Handle 'print' and 'expression' blocks
    else if (block->type == "print" || block->type == "expression") {
        ExpressionNode *root = block->condition->parseExpression();
        if (root != nullptr) {
            root->getVariablesNames();
            root->computeInfix();
            BooleanWrapper resultVar = root->computeResult();
            if (block->type == "print") {
                root->printResult(resultVar); // Print result for print blocks
            }
            delete root;
        }
    }
}

