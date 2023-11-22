
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
void printAST(Blocks *block, int indent = 0);
Blocks *buildAST(const std::vector<Tokens> & /* tokens */);
void printExpression(Blocks &block);
bool checkoperator(std::string op);
Blocks *parseStatements(std::vector<std::vector<Tokens>> &lines, size_t &lineIndex, size_t &tokenIndex);
Blocks *parseBlock(std::vector<std::vector<Tokens>> &lines, size_t &lineIndex, size_t &tokenIndex);
// parsestatements and parseblock will eventually have to be fixed. for now let it be a placeholder for constructing the AST
bool isVariable(std::string var)
{
    if (var == "true" || var == "false" || var == "print" || var == "if" || var == "while" || var == "else" || var == "def" || var == "null")
    {
        return false;
    }
    else if (!isdigit(var[0]))
    {
        for (char c : var)
        {
            if (c != '_' && !isalnum(c))
            {
                return false;
            }
        }
        return true;
    }
    return false;
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
        // size_t parens = 0;
        while (index + 1 < Lexer.tokenList.size())
        {
            // std::cout << "index is " << index << std::endl;
            // std::cout << "token at index is " << Lexer.tokenList.at(index).text << std::endl;
            if (Lexer.tokenList.at(index).text == "if" || Lexer.tokenList.at(index).text == "while" || Lexer.tokenList.at(index).text == "else" || Lexer.tokenList.at(index).text == "def")
            {
                // index++;
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
            else if ((!checkoperator(Lexer.tokenList.at(index).text) && Lexer.tokenList.at(index + 1).text == ",") || Lexer.tokenList.at(index).text == ",")
            {
                temp.push_back(Lexer.tokenList.at(index));
                index++;
            }
            else if ((!checkoperator(Lexer.tokenList.at(index).text) && Lexer.tokenList.at(index + 1).text == "("))
            {
                temp.push_back(Lexer.tokenList.at(index));
                index++;
            }
            else if (Lexer.tokenList.at(index).text == "(")
            {
                temp.push_back(Lexer.tokenList.at(index));
                index++;
            }
            else if (Lexer.tokenList.at(index).text == ";")
            {
                temp.push_back(Lexer.tokenList.at(index));
                tokensByLine.push_back(temp);
                temp.clear();
                index++;
            }
            else if (Lexer.tokenList.at(index).text == "print" || Lexer.tokenList.at(index).text == "return")
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
            else if ((Lexer.tokenList.at(index).text == ")") && Lexer.tokenList.at(index + 1).text == ";") // && !checkoperator(Lexer.tokenList.at(index + 1).text) || (!checkoperator(Lexer.tokenList.at(index).text) && Lexer.tokenList.at(index + 1).text == "(")
            {
                temp.push_back(Lexer.tokenList.at(index));
                temp.push_back(Lexer.tokenList.at(index + 1));
                tokensByLine.push_back(temp);
                temp.clear();
                //  temp.push_back(Lexer.tokenList.at(index + 1));
                index += 2;
            }
            else if ((Lexer.tokenList.at(index + 1).text == ")")) // && !checkoperator(Lexer.tokenList.at(index + 1).text) || (!checkoperator(Lexer.tokenList.at(index).text) && Lexer.tokenList.at(index + 1).text == "(")
            {
                temp.push_back(Lexer.tokenList.at(index));
                // temp.push_back(Lexer.tokenList.at(index + 1));
                // tokensByLine.push_back(temp);
                // temp.clear();
                //  temp.push_back(Lexer.tokenList.at(index + 1));
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
void printAST(Blocks *block, int indent)
{
    if (!block)
        return;

    std::string indentation(indent, ' ');

    if (block->type == "if" || block->type == "while")
    {
        std::cout << indentation << block->type << " ";
        printExpression(*block);
        std::cout << " {" << std::endl;

        for (const auto &childBlock : block->blocklist)
        {
            printAST(childBlock, indent + 4);
        }
        std::cout << indentation << "}" << std::endl;
    }
    else if (block->type == "function_call")
    {
        std::cout << indentation << block->name;
        for (Tokens token : block->condition->getTokens())
        {
            if (token.text != " ")
            {
                std::cout << token.text;
                if (token.text == ",")
                {
                    std::cout << " ";
                }
            }
        }
        // std::cout << ";" << std::endl;
    }
    else if (block->type == "function_def")
    {
        std::cout << indentation << "def " << block->name;
        for (Tokens token : block->condition->getTokens())
        {
            std::cout << token.text;
            if (token.text == ",")
            {
                std::cout << " ";
            }
        }
        std::cout << " {" << std::endl;
        for (const auto &childBlock : block->blocklist)
        {
            printAST(childBlock, indent + 4);
        }
        std::cout << indentation << "}" << std::endl;
    }
    else if (block->type == "print" || block->type == "expression" || block->type == "return")
    {
        if (block->condition)
        {
            if (block->condition->getTokens()[0].text != "}")
            {
                std::cout << indentation;
                if (block->type == "print" || block->type == "return")
                    std::cout << block->type;
                if ((block->type == "return" && block->condition->getTokens().size() != 1) || block->type == "print")
                {

                    std::cout << " ";
                    printExpression(*block);
                }
                else if (block->type == "expression")
                {
                    printExpression(*block);
                }
                std::cout << ";" << std::endl;
            }
        }
        else
        {
            std::cout << indentation;
            if (block->type == "print" || block->type == "return")
                std::cout << block->type;
            if (!block->blocklist.empty())
            {
                std::cout << " ";
                printAST(block->blocklist[0], 0);
            }
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

// Implement this function based on your ExpressionParser and how it represents expressions
void printExpression(Blocks &block)
{
    if (!block.blocklist.empty())
    {
        for (Tokens token : block.condition->tokens)
        {
            bool found = false;
            for (Blocks *bblock : block.blocklist)
            {
                if (bblock->type == "function_call" && bblock->name == token.text)
                {
                    std::cout << bblock->name << "(";
                    for (Tokens token : bblock->condition->tokens)
                    {
                        std::cout << token.text;
                        if (token.text == ",")
                        {
                            std::cout << " ";
                        }
                    }
                    std::cout << ")";
                    found = true;
                    // pos++;
                }
            }
            if (!found)
            {
                if (token.text != ";")
                {
                    if (checkoperator(token.text))
                    {
                        std::cout << " " << token.text << " ";
                    }
                    else
                    {
                        std::cout << token.text;
                    }
                }
            }
        }
    }
    else
    {
        ExpressionParser *exp = block.condition;
        ExpressionNode *root = exp->parseExpression();
        if (root != nullptr)
        {
            root->getVariablesNames();
            root->computeInfix();
            root->printInfix(false);
            delete root;
        }
    }
}

// bool checkoperator(std::string op)
// {
//     std::vector<std::string> ops = {
//         "+", "-", "*", "/", "=", "%",
//         "==", ">", ">=", "<", "<=", "|", "^", "&", "!=",
//         "+", "-", "*", "/", "=", "%", "==", ">",
//         ">=", "<", "<=", "|", "^", "&", "!=",
//         "+", "-", "*", "/", "=", "%", "==", ">", ">=",
//         "<", "<=", "|", "^", "&", "!=", "(", ")", ","};
//     if (std::find(ops.begin(), ops.end(), op) == ops.end())
//     {
//         return false;
//     }
//     return true;
// }
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
        // if (tokens.back().text == ";")
        // {
        block->type = "print";
        std::vector<Tokens> printExpressionTokens(tokens.begin() + 1, tokens.end());
        size_t index = tokenIndex;
        std::string text;
        while (tokens[index].text != "(" && index < tokens.size())
        {
            if (isVariable(tokens[index].text))
            {
                text += tokens[index].text;
            }
            index++;
        }
        if (tokens[index].text == "(" && isVariable(tokens[index - 1].text))
        {
            Blocks *blocky = new Blocks();
            blocky->type = "function_call";
            blocky->name = text;
            std::vector<Tokens> func_vars;
            while (tokens[index].text != ")")
            {
                func_vars.push_back(tokens[index]); // includes both ( ) and commas
                index++;
            }
            func_vars.push_back(tokens[index]);
            blocky->condition = new ExpressionParser(func_vars);
            block->blocklist.push_back(blocky);
            // tokenIndex = index;
        }
        else
        {
            block->condition = new ExpressionParser(printExpressionTokens);
        }
        lineIndex++;
        return block;
        // }
        // else
        // {
        //     std::string error_message = "Unexpected token at line " + std::to_string(lineIndex) + " column " + std::to_string(tokenIndex);
        //     throw std::runtime_error(error_message);
        // }
    }
    else if (tokens.front().text == "def")
    {
        block->type = "function_def";
        int index;
        block->name = tokens[1].text;
        for (size_t i = 2; i < tokens.size(); i++)
        {
            if (tokens[i].text == "(")
            {
                index = i;
                break;
            }
        }
        std::vector<Tokens> func_vars;
        while (tokens[index].text != ")")
        {
            func_vars.push_back(tokens[index]); // includes both ( ) and commas
            index++;
        }
        func_vars.push_back(tokens[index]);
        block->condition = new ExpressionParser(func_vars);

        lineIndex++;
        tokenIndex = 0;
        while (lineIndex < lines.size() && lines[lineIndex].back().text != "}")
        {
            block->blocklist.push_back(parseStatements(lines, lineIndex, tokenIndex));
        }
        block->blocklist.push_back(parseStatements(lines, lineIndex, tokenIndex)); // here

        return block;
    }
    else if (tokens.front().text == "return")
    {
        block->type = "return";
        std::vector<Tokens> printExpressionTokens(tokens.begin() + 1, tokens.end());
        block->condition = new ExpressionParser(printExpressionTokens);
        lineIndex++;
        return block;
    }
    else
    {
        block->type = "expression";
        std::vector<Tokens> printExpressionTokens(tokens.begin(), tokens.end());
        size_t index = 0;
        std::string text;
        while (tokens[index].text != "(" && index < tokens.size())
        {
            text += tokens[index].text;
            // if (tokens[index].text)
            // {
            //     text.clear();
            // }
            // for (char character : text)
            // {
            if (checkoperator(tokens[index].text))
            {
                text.clear();
            }
            // }
            index++;
        }
        if (tokens[index].text == "(" && isVariable(tokens[index - 1].text))
        {
            Blocks *func_call_block = new Blocks();
            func_call_block->type = "function_call";
            func_call_block->name = text;
            std::vector<Tokens> func_vars;
            while (tokens[index].text != ")")
            {
                func_vars.push_back(tokens[index]); // includes both ( ) and commas
                index++;
            }
            func_vars.push_back(tokens[index]);
            func_call_block->condition = new ExpressionParser(func_vars);
            lineIndex++;
            block->blocklist.push_back(func_call_block);
            // tokenIndex = index;
        }
        // if (block->type == "function_call")
        // {
        //     Blocks *exp_block = new Blocks();
        //     exp_block->type = "expression";
        //     exp_block->blocklist.push_back(exp_block);
        //     exp_block->condition = new ExpressionParser(tokens);
        //     lineIndex++;
        //     return exp_block;
        // }
        block->condition = new ExpressionParser(tokens);
        lineIndex++;
        tokenIndex = index;
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