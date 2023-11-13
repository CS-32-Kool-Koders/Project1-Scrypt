
#include "lexer.h"
#include <ctype.h>
#include <iostream>
#include <algorithm>

// in lex.cpp in src, we have a while loop that goes thru input line-by-line
// add goes thu each char
// making tokens out of them

int lexer::tokenize(int row, std::string line)
{
    // takes a line and the row it is on as input

    // regarding tokenList.push_back(Tokens(row, (int)i, std::to_string(1, line[i])));
    //-> using to_string method on line[i], because Tokens constructor needs 3rd argument to be a string
    //-> using (int) method on i+1 and row, because Tokens constructor needs 1st and 2nd argument to be an int

    // This will register a blank line as a token
    // if (line.size() == 0 || std::all_of(line.begin(), line.end(), isspace))
    // {
    //     tokenList.push_back(Tokens((int)row, 1, ""));
    //     return 0;
    // }

    // goes through line character by character
    for (size_t i = 0; i < line.size(); i++)
    {
        // case - plus
        if (line[i] == '+')
        {
            tokenList.push_back(Tokens((int)row, (int)i + 1, std::string(1, line[i])));
        }
        // case - minus
        else if (line[i] == '-')
        {
            tokenList.push_back(Tokens((int)row, (int)i + 1, std::string(1, line[i])));
        }
        // case - multiply
        else if (line[i] == '*')
        {
            tokenList.push_back(Tokens((int)row, (int)i + 1, std::string(1, line[i])));
        }
        // case - divide
        else if (line[i] == '/')
        {
            tokenList.push_back(Tokens((int)row, (int)i + 1, std::string(1, line[i])));
        }
        // case - left_paren
        else if (line[i] == '(')
        {
            tokenList.push_back(Tokens((int)row, (int)i + 1, std::string(1, line[i])));
        }
        // case - right_paren
        else if (line[i] == ')')
        {
            tokenList.push_back(Tokens((int)row, (int)i + 1, std::string(1, line[i])));
        }
        else if (line.substr(i, 2) == ">=")
        {
            tokenList.push_back(Tokens((int)row, (int)i + 1, ">="));
            i++;
        }
        else if (line.substr(i, 2) == "<=")
        {
            tokenList.push_back(Tokens((int)row, (int)i + 1, "<="));
            i++;
        }
        else if (line.substr(i, 2) == "==")
        {
            tokenList.push_back(Tokens((int)row, (int)i + 1, "=="));
            i++;
        }
        else if (line.substr(i, 2) == "!=")
        {
            tokenList.push_back(Tokens((int)row, (int)i + 1, "!="));
            i++;
        }
        else if (line[i] == '>')
        {
            tokenList.push_back(Tokens((int)row, (int)i + 1, ">"));
        }
        else if (line[i] == '<')
        {
            tokenList.push_back(Tokens((int)row, (int)i + 1, "<"));
        }
        else if (line[i] == '&')
        {
            tokenList.push_back(Tokens((int)row, (int)i + 1, "&"));
        }
        else if (line[i] == '^')
        {
            tokenList.push_back(Tokens((int)row, (int)i + 1, "^"));
        }
        else if (line[i] == '|')
        {
            tokenList.push_back(Tokens((int)row, (int)i + 1, "|"));
        }
        else if (line[i] == '%')
        {
            tokenList.push_back(Tokens((int)row, (int)i + 1, "%"));
        }
        else if (line[i] == '{')
        {
            tokenList.push_back(Tokens((int)row, (int)i + 1, "{"));
        }
        else if (line[i] == '}')
        {
            tokenList.push_back(Tokens((int)row, (int)i + 1, "}"));
        }
        else if (line[i] == ';'){
            tokenList.push_back(Tokens((int)row, (int)i + 1, ";"));
        }
        else if (line[i] == ','){
            tokenList.push_back(Tokens((int)row, (int)i + 1, ","));
        }
         else if (line[i] == '['){
            tokenList.push_back(Tokens((int)row, (int)i + 1, ","));
        }
         else if (line[i] == ','){
            tokenList.push_back(Tokens((int)row, (int)i + 1, "]"));
        }
        // case - space or similar character
        else if (isspace(line[i]))
        {
        }
        // case - dealing with decimal/number cases
        else if (isdigit(line[i]) || line[i] == '.')
        {
            checkIsDigit(line, (int)row, (int)i);
            // resetting i to the next character
            i = tokenList.back().col + tokenList.back().text.size() - 2;
        }
        else if (line[i] == '=')
        {
            tokenList.push_back(Tokens((int)row, (int)i + 1, std::string(1, line[i])));
        }
        else if (line.substr(i, 2) == "if")
        {
            tokenList.push_back(Tokens((int)row, (int)i + 1, "if"));
            i++;
        }
        else if (line.substr(i, 5) == "while")
        {
            tokenList.push_back(Tokens((int)row, (int)i + 1, "while"));
            i = i + 4;
        }
        else if (line.substr(i, 4) == "else")
        {
            tokenList.push_back(Tokens((int)row, (int)i + 1, "else"));
            i = i + 3;
        }
        else if (line.substr(i, 5) == "print")
        {
            tokenList.push_back(Tokens((int)row, (int)i + 1, "print"));
            i = i + 4;
        }

        else if (isalpha(line[i]) || line[i] == '_')
        {
            i = checkIsIdentifier(line, (int)row, (int)i);
        }

        else
        {
            // case - invalid character
            //  std::cout << "Syntax error on line " << row << " column " << i+1 << "." << std::endl;
            //  // exit(1);
            //  throw;
            throw std::runtime_error("Syntax error on line " + std::to_string(row) + " column " + std::to_string(i + 1) + ".");
            std::cout << "Syntax error on line " << row << " column " << i + 1 << "." << std::endl;
        }
    }

    return 0;
}

int lexer::checkIsDigit(std::string line, int row, int col)
{
    // takes a line, the row its on, and the colummn
    int decimal = 0; // number of decimals in number
    int offset = 0;  // to offset col to the correct digit column
    // type string in case digit has a decimal
    std::string digit; // stores the resulting digit as we go through the line
    if (isdigit(line[col]))
    {
        digit += line[col];
        while (isdigit(line[col + 1]) || line[col + 1] == '.')
        {
            if (line[col + 1] == '.' && decimal > 0)
            {
                // multiple decimal case
                //  std::cout << "Syntax error on line " << row << " column " << col+2 << "." << std::endl;
                //  // exit(1);
                //  throw;
                throw std::runtime_error("Syntax error on line " + std::to_string(row) + " column " + std::to_string(col + 2) + ".");
                std::cout << "Syntax error on line " << row << " column " << col + 2 << "." << std::endl;
            }
            else if (line[col + 1] == '.')
            {
                if (!isdigit(line[col + 2]))
                {
                    // decimal at end of number case
                    //  std::cout << "Syntax error on line " << row << " column " << col+3 << "." << std::endl;
                    //  // exit(1);
                    //  throw;
                    throw std::runtime_error("Syntax error on line " + std::to_string(row) + " column " + std::to_string(col + 3) + ".");
                    std::cout << "Syntax error on line " << row << " column " << col + 3 << "." << std::endl;
                }
                // number has a decimal
                decimal++; // number of decimals in number
                digit += line[col + 1];
                col++;
                offset += 1;
            }
            else
            {
                // just another number
                digit += line[col + 1];
                col++;
                offset += 1;
            }
        }
    }
    else
    {
        // starting decimal case
        //  std::cout << "Syntax error on line " << row << " column " << col+1 << "." << std::endl;
        //  // exit(1);
        //  throw;
        throw std::runtime_error("Syntax error on line " + std::to_string(row) + " column " + std::to_string(col + 1) + ".");
        std::cout << "Syntax error on line " << row << " column " << col + 1 << "." << std::endl;
    }
    // col-offset+1 to reset the coords of column of the number
    tokenList.push_back(Tokens(row, col - offset + 1, digit));
    return 0;
}
int lexer::checkIsIdentifier(std::string line, int row, int col)
{
    std::string identifier;
    identifier += line[col];
    while (isalnum(line[col + 1]) || line[col + 1] == '_')
    {
        identifier += line[col + 1];
        col++;
    }
    tokenList.push_back(Tokens(row, col - identifier.size() + 2, identifier));
    return col;
}