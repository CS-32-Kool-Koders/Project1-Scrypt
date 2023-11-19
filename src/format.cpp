
#include <iostream>
#include <iomanip>
#include "lib/infix_parser.h"
#include "lib/lexer.h"
#include <string>
#include <sstream>
#include <vector>
#include "blocks.cpp"

// Assuming 'Tokens', 'Blocks', and other necessary classes are defined in the included headers

// Forward declarations
void printAST(const Blocks *block, int indent = 0);
Blocks *buildAST(const std::vector<Tokens> & /* tokens */);
void printExpression(ExpressionParser *exp /* expr */);
bool checkoperator(std::string op);
Blocks *parseStatements(std::vector<std::vector<Tokens>> &lines, size_t &lineIndex, size_t &tokenIndex);
Blocks *parseBlock(std::vector<std::vector<Tokens>> &lines, size_t &lineIndex, size_t &tokenIndex);
// parsestatements and parseblock will eventually have to be fixed. for now let it be a placeholder for constructing the AST

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
        // size_t parens = 0;
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
            else if (Lexer.tokenList.at(index).text == ";")
            {
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

            else if ((Lexer.tokenList.at(index + 1).text == ")" && !checkoperator(Lexer.tokenList.at(index + 1).text)) || (!checkoperator(Lexer.tokenList.at(index).text) && Lexer.tokenList.at(index + 1).text == "("))
            {
                temp.push_back(Lexer.tokenList.at(index));
                tokensByLine.push_back(temp);
                temp.clear();
                // temp.push_back(Lexer.tokenList.at(index + 1));
                index++;
            }
            // else if (Lexer.tokenList.at(index).text == ")")
            // {
            //     temp.push_back(Lexer.tokenList.at(index));
            //     index++;
            // }

            // else if (Lexer.tokenList.at(index).text == "(" && !checkoperator(Lexer.tokenList.at(index + 1).text))
            // {
            //     parens++;
            //     temp.push_back(Lexer.tokenList.at(index));
            //     temp.push_back(Lexer.tokenList.at(index + 1));
            //     index += 2;
            // }
            else if (checkoperator(Lexer.tokenList.at(index).text) && !checkoperator(Lexer.tokenList.at(index + 1).text))
            {
                if (Lexer.tokenList.at(index).text == ")")
                {
                    temp.push_back(Lexer.tokenList.at(index));
                    tokensByLine.push_back(temp);
                    temp.clear();
                    index++;
                }
                temp.push_back(Lexer.tokenList.at(index));
                temp.push_back(Lexer.tokenList.at(index + 1));
                index += 2;
            }
            else if (checkoperator(Lexer.tokenList.at(index).text) && checkoperator(Lexer.tokenList.at(index + 1).text))
            {
                temp.push_back(Lexer.tokenList.at(index));
                temp.push_back(Lexer.tokenList.at(index + 1));
                index += 2;
            }
            else if (Lexer.tokenList.at(index).text == ")")
            {
                temp.push_back(Lexer.tokenList.at(index));
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
                else if (Lexer.tokenList.at(index + 1).text == ";")
                {
                    if (index + 2 < Lexer.tokenList.size() && Lexer.tokenList.at(index + 2).text == "}")
                    {
                        temp.push_back(Lexer.tokenList.at(index));
                        temp.push_back(Lexer.tokenList.at(index + 1));
                        temp.push_back(Lexer.tokenList.at(index + 2));
                        tokensByLine.push_back(temp);
                        temp.clear();
                        // temp.push_back(Lexer.tokenList.at(index + 1));
                        index += 3;
                    }
                    else
                    {
                        temp.push_back(Lexer.tokenList.at(index));
                        temp.push_back(Lexer.tokenList.at(index + 1));
                        tokensByLine.push_back(temp);
                        temp.clear();
                        // temp.push_back(Lexer.tokenList.at(index + 1));
                        index += 2;
                    }
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
            else if (!checkoperator(Lexer.tokenList.at(index).text))
            {
                temp.push_back(Lexer.tokenList.at(index));
                tokensByLine.push_back(temp);
                temp.clear();
                // temp.push_back(Lexer.tokenList.at(index + 1));
                index++;
            }
            else
            {
                index++;
            }
        }
        if (!temp.empty())
        {
            tokensByLine.push_back(temp);
            temp.clear();
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
            printAST(rootBlock);
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
}

// Implement this function to build the AST from the tokens
Blocks *buildAST(const std::vector<Tokens> & /* tokens */)
{
    // Add your logic to build the AST from the tokens
    return new Blocks(); // Placeholder return
}

// Recursive function to print the AST with indentation
void printAST(const Blocks *block, int indent)
{
    if (!block)
        return;

    std::string indentation(indent, ' ');

    if (block->type == "if" || block->type == "while")
    {
        std::cout << indentation << block->type << " ";
        printExpression(block->condition);
        std::cout << " {" << std::endl;

        for (const auto &childBlock : block->blocklist)
        {
            printAST(childBlock, indent + 4);
        }
        std::cout << indentation << "}" << std::endl;
    }
    else if (block->type == "print" || block->type == "expression")
    {
        if (block->condition->getTokens()[0].text != "}")
        {
            std::cout << indentation;
            if (block->type == "print")
                std::cout << block->type << " ";
            printExpression(block->condition);
            std::cout << ";" << std::endl;
        }
    }
    else if (block->type == "else")
    {
        // Handle the else block
        std::cout << indentation << "else {" << std::endl;
        for (const auto &childBlock : block->blocklist)
        {
            printAST(childBlock, indent + 4);
        }
        std::cout << indentation << "}" << std::endl;
    }
}

// Implement this function based on your ExpressionParser and how it represents expressions
void printExpression(ExpressionParser *exp)
{
    // Your logic to traverse and print the expression
    ExpressionNode *root = exp->parseExpression();
    if (root != nullptr)
    {
        root->getVariablesNames();
        root->computeInfix();
        root->printInfix(false);
        delete root;
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
        "<", "<=", "|", "^", "&", "!=", "(", ")"};
    if (std::find(ops.begin(), ops.end(), op) == ops.end())
    {
        return false;
    }
    return true;
}
// gonna have to change both the below statements later
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

        // // Handle 'else' or 'else if'
        // if (lineIndex < lines.size() && lines[lineIndex].front().text == "else")
        // {
        //     // maybe delete the bottom two lines, don't go to next line for else if
        //     // move to next token, see if next token is an if
        //     //  lineIndex++;
        //     //  tokenIndex = 0; // HMMMMMMMMM
        //     //  Blocks *block = new Blocks();
        //     if (lineIndex < lines.size() && lines[lineIndex].front().text == "if")
        //     {
        //         // This is an 'else if' block
        //         // elseBlock
        //         block->blocklist.push_back(parseStatements(lines, lineIndex, tokenIndex));
        //         return block;
        //         // block = parseStatements(lines, lineIndex, tokenIndex);
        //     }
        //     else
        //     {
        //         // This is an 'else' block
        //         tokenIndex = 0;
        //         // elseBlock
        //         // OVERHERE
        //         block->blocklist.push_back(parseStatements(lines, lineIndex, tokenIndex));
        //         return block;
        //         // block = parseStatements(lines, lineIndex, tokenIndex);
        //     }
        // }
    }
    else if (tokens.front().text == "else")
    {
        block->type = "else";
        int elseNum = 0;
        while (lineIndex < lines.size() && lines[lineIndex].front().text == "else")
        {
            tokens = lines[lineIndex];
            if (tokens[1].text == "if")
            {
                elseNum++;
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
                // std::cout << "THIS IS TH ELINE INDEX: " << lineIndex << std::endl;
                //  lineIndex--;
                //    return block;
            }
            else
            {
                elseNum++;
                if (elseNum > 1)
                {
                    Blocks *Eblock = new Blocks();
                    Eblock->type = "else";
                    lineIndex++;
                    while (lineIndex < lines.size() && lines[lineIndex].back().text != "}")
                    {
                        // std::cout << "This is the thing " << lines[lineIndex ].back().text << std::endl;
                        Eblock->blocklist.push_back(parseStatements(lines, lineIndex, tokenIndex));
                        // lineIndex++;
                    }
                    // std::cout << "THIS IS FOR DEBUGGING }: " << lines[lineIndex].back().text;
                    Eblock->blocklist.push_back(parseStatements(lines, lineIndex, tokenIndex)); // here here
                    block->blocklist.push_back(Eblock);
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
                }
            }
        }
        return block;
    }
    else if (tokens.front().text == "print")
    {
        if (tokens.back().text == ";")
        {
            block->type = "print";
            std::vector<Tokens> printExpressionTokens(tokens.begin() + 1, tokens.end());
            block->condition = new ExpressionParser(printExpressionTokens);
            lineIndex++;
            return block;
        }
        else
        {
            std::string error_message = "Unexpected token at line " + std::to_string(tokens.front().col) + " column " + std::to_string(tokens.front().line);
            throw std::runtime_error(error_message);
        }
    }
    else
    {
        // if (tokens.back().text == ";")
        // {
        block->type = "expression";
        block->condition = new ExpressionParser(tokens);
        lineIndex++;
        return block;
        // }
        // else
        // {
        //     std::cout << "index: " << lineIndex;
        //     for (auto token : tokens)
        //     {
        //         std::cout << token.text << " " << std::endl;
        //     }
        //     std::string error_message = "2Unexpected token at line " + std::to_string(tokens.front().col + 1) + " column " + std::to_string(lineIndex);
        //     throw std::runtime_error(error_message);
        // }
    }

    lineIndex++;
    return block;
}