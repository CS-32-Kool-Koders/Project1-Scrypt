
#include <iostream>
// #include "lib/infix_parser.h"
#include "lib/lexer.h"
#include "blocks.cpp"
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

        // std::cout << "THIS IS FOR DEBUGGING }: " << lines[lineIndex].back().text;
        block->blocklist.push_back(parseStatements(lines, lineIndex, tokenIndex)); // here
        // block = parseStatements(lines, lineIndex, tokenIndex);
        return block;

        // Handle 'else' or 'else if'
        if (lineIndex < lines.size() && lines[lineIndex].front().text == "else")
        {
            // maybe delete the bottom two lines, don't go to next line for else if
            // move to next token, see if next token is an if
            //  lineIndex++;
            //  tokenIndex = 0; // HMMMMMMMMM
            //  Blocks *block = new Blocks();
            if (lineIndex < lines.size() && lines[lineIndex].front().text == "if")
            {
                // This is an 'else if' block
                // elseBlock
                block->blocklist.push_back(parseStatements(lines, lineIndex, tokenIndex));
                return block;
                // block = parseStatements(lines, lineIndex, tokenIndex);
            }
            else
            {
                // This is an 'else' block
                tokenIndex = 0;
                // elseBlock
                // OVERHERE
                block->blocklist.push_back(parseStatements(lines, lineIndex, tokenIndex));
                return block;
                // block = parseStatements(lines, lineIndex, tokenIndex);
            }
        }
    }
    else if (tokens.front().text == "else")
    {
        block->type = "else";
        if (tokens[1].text == "if")
        {
            Blocks *ifblock = new Blocks();
            ifblock->type = "if";
            std::vector<Tokens> conditionTokens(tokens.begin() + 2, tokens.end() - 1); // Exclude 'if'/'while' and '{'
            ifblock->condition = new ExpressionParser(conditionTokens);

            lineIndex++;
            tokenIndex = 0;
            while (lineIndex < lines.size() && lines[lineIndex].back().text != "}")
            {
                // std::cout << "This is the thing " << lines[lineIndex ].back().text << std::endl;
                ifblock->blocklist.push_back(parseStatements(lines, lineIndex, tokenIndex));
                // lineIndex++;
            }

            // std::cout << "THIS IS FOR DEBUGGING }: " << lines[lineIndex].back().text;
            ifblock->blocklist.push_back(parseStatements(lines, lineIndex, tokenIndex)); // here
            // block = parseStatements(lines, lineIndex, tokenIndex);
            // return block;
            block->blocklist.push_back(ifblock);
        }
        else
        {
            lineIndex++;
            while (lineIndex < lines.size() && lines[lineIndex].back().text != "}")
            {
                // std::cout << "This is the thing " << lines[lineIndex ].back().text << std::endl;
                block->blocklist.push_back(parseStatements(lines, lineIndex, tokenIndex));
                // lineIndex++;
            }
            // std::cout << "THIS IS FOR DEBUGGING }: " << lines[lineIndex].back().text;
            block->blocklist.push_back(parseStatements(lines, lineIndex, tokenIndex)); // here here
            return block;
        }
        // lineIndex++;
        // // if (lineIndex >= lines.size())
        // // {
        // //     return block;
        // // }
        // while (lineIndex < lines.size() && lines[lineIndex].back().text != "}")
        // {
        //     // std::cout << "This is the thing " << lines[lineIndex ].back().text << std::endl;
        //     block->blocklist.push_back(parseStatements(lines, lineIndex, tokenIndex));
        //     // lineIndex++;
        // }
        // // std::cout << "THIS IS FOR DEBUGGING }: " << lines[lineIndex].back().text;
        // block->blocklist.push_back(parseStatements(lines, lineIndex, tokenIndex)); // here here
        // return block;
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

        for (Blocks *rootBlock : astNodes)
        {
            evaluateBlock(rootBlock);
        }
        for (Blocks *root : astNodes)
        {
            delete root;
        }

        return 0;
    }
    catch (const std::runtime_error &e)
    {
        std::cout << e.what() << std::endl;
        exit(1);
    }
    catch (const std::logic_error &e)
    {
        std::cout << e.what() << std::endl;
        exit(1);
    }
}

void evaluateBlock(Blocks *block)
{
    if (!block)
        return;

    // Handle 'print' and 'expression' blocks
    if (block->type == "print" || block->type == "expression")
    {
        ExpressionNode *root = block->condition->parseExpression();
        // std::cout<<root<<std::endl;
        if (root != nullptr)
        {
            root->getVariablesNames();
            root->computeInfix();
            BooleanWrapper resultVar1 = root->computeResult();
            // root->printResult(resultVar1); // Print result for print blocks
            if (block->type == "print")
            {
                root->printResult(resultVar1); // Print result for print blocks
            }
            delete root;
            // return;
        }
    }
    // For 'if' and 'while' blocks, evaluate the condition
    else if (block->type == "if" || block->type == "while")
    {
        ExpressionNode *root = block->condition->parseExpression();
        BooleanWrapper resultVar2;
        if (root != nullptr)
        {
            root->getVariablesNames();
            root->computeInfix();
            resultVar2 = root->computeResult();
            delete root;
        }

        // Evaluate 'if' blocks
        if (block->type == "if")
        {
            if (resultVar2.getBvalue() && !block->blocklist.empty())
            {
                for (auto *bl : block->blocklist)
                {
                    evaluateBlock(bl);
                }
            }
            else
            {
                for (auto *bl : block->blocklist)
                {
                    if (bl->type == "else")
                    {
                        evaluateBlock(bl);
                    }
                }
            }
        }
        // Evaluate 'while' blocks
        else if (block->type == "while")
        {
            while (resultVar2.getBvalue())
            {
                // Execute each block in the blocklist
                // std::cout<<resultVar2.getBvalue()<<std::endl;
                for (Blocks *innerBlock : block->blocklist)
                {
                    std::vector<Tokens> tempVec;
                    if (!innerBlock)
                    {
                        break;
                    }
                    if (innerBlock->condition)
                    {
                        for (auto i : innerBlock->condition->tokens)
                        {
                            if (i.text != "}")
                            {
                                tempVec.push_back(i);
                                // std::cout<< "Token: " << i.text<< std::endl;
                            }
                        }
                        innerBlock->condition->tokens = tempVec;
                    }
                    else
                    {
                        std::cout << "this should break" << std::endl;
                        break;
                    }
                    // //std::cout <<std::endl;
                    // for (auto i: innerBlock->condition->tokens){
                    //     //std::cout<< "New Token: " << i.text<< std::endl;
                    // }

                    // std::cout<<innerBlock<< innerBlock->type<<std::endl;
                    evaluateBlock(innerBlock);
                }

                // Re-evaluate the condition at the end of each loop iteration
                root = block->condition->parseExpression();
                if (root != nullptr)
                {
                    root->getVariablesNames();
                    root->computeInfix();
                    resultVar2 = root->computeResult();
                    delete root;
                }
            }
        }
    }
    // Handle 'else' blocks
    // this shit wrong prolly, come back to
    // else if (block->type == "else") {
    //     for (Blocks *innerBlock : block->blocklist) {
    //         evaluateBlock(innerBlock);
    //     }
    // }
}
