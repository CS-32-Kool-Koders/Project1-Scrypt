#include <iostream>
#include <vector>
#include "infix_parser.h"
#include "tokens.h"
#include <algorithm>

std::vector<std::string> supportedOperators = {
    "+", "-", "*", "/", "=", "END", "%", "==", ">", ">=", "<", "<=", "|", "^", "&", "!="};

std::vector<std::string> ExpressionParser::knowsVariables;
std::map<std::string, bool> ExpressionParser::boolVariables;
std::map<std::string, double> ExpressionParser::variables;
std::string ExpressionParser::line;

double result;
int eqNb = 0;
int column = 1;
std::stringstream strstrm;
void ExpressionNode::computeInfix()
{
    if (left != nullptr && right != nullptr)
    {
        strstrm << "(";
        left->computeInfix();
        if (std::find(supportedOperators.begin(), supportedOperators.end(), value) != supportedOperators.end())
        {
            strstrm << " " << value << " ";
        }
        else
        {
            strstrm << value;
        }
        right->computeInfix();
        strstrm << ")";
    }
    else
    {
        if (!isVariable(value))
        {
            if (std::floor(std::stod(value)) == std::stod(value))
            {
                strstrm << std::floor(std::stod(value));
            }
            else
            {
                strstrm << std::stod(value);
            }
        }
        else
        {
            strstrm << value;
        }
    }
}

void ExpressionNode::checkParentheses(std::string tokenString)
{

    int paren_count = 0;

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
}

void ExpressionNode::printTree()
{
    std::cout << strstrm.str() << std::endl;
    strstrm.str("");
    strstrm.clear();
}

ExpressionNode *ExpressionParser::parseExpression()
{
    return parseAssignment();
}

ExpressionNode *ExpressionParser::parseAssignment()
{
    ExpressionNode *left = parseAddSubtract();
    if (currentIndex < tokens.size() && tokens[currentIndex].text == "=")
    {
        currentIndex++;
        ExpressionNode *right = parseAssignment();
        ExpressionNode *node = new ExpressionNode("=");
        node->left = left;
        node->right = right;
        return node;
    }
    return left;
}

ExpressionNode *ExpressionParser::parseAddSubtract()
{
    return parseOperator(std::bind(&ExpressionParser::parseMultiplyDivide, this), std::vector<std::string>{"+", "-"});
}

ExpressionNode *ExpressionParser::parseMultiplyDivide()
{
    return parseOperator(std::bind(&ExpressionParser::parseComparison, this), std::vector<std::string>{"*", "/", "%"});
}

ExpressionNode *ExpressionParser::parseComparison()
{
    return parseOperator(std::bind(&ExpressionParser::parseOperand, this), std::vector<std::string>{"==", ">", ">=", "<", "<=", "|", "^", "&", "!="});
}

ExpressionNode *ExpressionParser::parseOperator(std::function<ExpressionNode *()> parseFunction, std::vector<std::string> operators)
{
    ExpressionNode *left = parseFunction();
    while (currentIndex < tokens.size() && std::find(operators.begin(), operators.end(), tokens[currentIndex].text) != operators.end())
    {
        std::string operatorText = tokens[currentIndex].text;
        currentIndex++;
        ExpressionNode *right = parseFunction();
        ExpressionNode *node = new ExpressionNode(operatorText);
        node->left = left;
        node->right = right;
        left = node;
    }
    return left;
}

ExpressionNode *ExpressionParser::parseOperand()
{
    if (currentIndex < tokens.size())
    {
        std::string text = tokens[currentIndex].text;
        currentIndex++;
        if (text == "(")
        {
            ExpressionNode *node = parseAssignment();
            if (currentIndex < tokens.size() && tokens[currentIndex].text == ")")
            {
                currentIndex++;
                return node;
            }
            if (node != nullptr)
                delete node;
        }
        else
        {
            return new ExpressionNode(text);
        }
    }
    return nullptr;
}

bool ExpressionNode::isVariable(std::string var)
{
    if (var == "true" || var == "false")
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

void ExpressionNode::getVariablesNames()
{
    if (right == nullptr)
        return;

    if (value == "=")
    {
        eqNb++;
        if (isVariable(left->value))
        {
            ExpressionParser::knowsVariables.push_back(left->value);
        }
        else
        {
            // use eqNb (which is incremented each time an = is found) to find the colum of the = from ExpressionParser::line
            int eqColumn = 0;
            for (std::size_t i = 0; i < ExpressionParser::line.length(); i++)
            {
                if (ExpressionParser::line[i] != '=')
                    eqColumn++;
                else
                    break;
            }
            throw std::logic_error("Unexpected token at line 1 column " + std::to_string(eqColumn + 1) + ": " + value);
        }
    }

    right->getVariablesNames();
}

BooleanWrapper ExpressionNode::computeResult()
{
    // if (value == "+" || value == "-" || value == "*" || value == "/" || value == "=" ||
    //     value == "END" || value == "%" || value == "==" || value == ">" || value == ">=" ||
    //     value == "<" || value == "<=" || value == "|" || value == "^" || value == "&" || value == "!=")
    // {
    if (std::find(supportedOperators.begin(), supportedOperators.end(), value) != supportedOperators.end())
    {
        if (value == "=" && right != nullptr && right->value == "END")
        {
            int endColumn = ExpressionParser::line.find("END");
            throw std::logic_error("Unexpected token at line 1 column " + std::to_string(endColumn + 1) + ": END");
        }
        if (left == nullptr || right == nullptr)
        {
            //|| (left->value == "END" || right->value == "END")
            // if(left->value == "END") {
            //     std::string throw_message = "Unexpected token at line 1 column " + std::to_string(column) + ": " + value;
            //     column = 1;
            //     throw std::logic_error(throw_message);
            // }
            // std::cout << "i am here" << std::endl;
            std::string throw_message = "Unexpected token at line 1 column " + std::to_string(column) + ": " + value;
            column = 1;
            throw std::logic_error(throw_message);
        }
        // std::cout << "i was here" << std::endl;
        BooleanWrapper leftValue = left->computeResult();
        column += 4;
        BooleanWrapper rightValue = right->computeResult();

        // column++;
        if (value == "+")
        {
            return leftValue + rightValue;
        }
        else if (value == "-")
        {
            return leftValue - rightValue;
        }
        else if (value == "*")
        {
            // std::cout << column << " is multiply " << std::endl;
            return leftValue * rightValue;
        }
        else if (value == "/")
        {
            return leftValue / rightValue;
        }
        else if (value == "=")
        {
            if (rightValue.printType() == 'B')
            {
                ExpressionParser::boolVariables[left->value] = rightValue.getBvalue();
            }
            else if (rightValue.printType() == 'D')
            {
                ExpressionParser::variables[left->value] = rightValue.getDvalue();
            }

            return rightValue;
        }
        else if (value == "%")
        {
            return rightValue % leftValue;
        }
        else if (value == "==")
        {
            return rightValue == leftValue;
        }
        else if (value == ">")
        {
            return rightValue > leftValue;
        }
        else if (value == ">=")
        {
            return rightValue >= leftValue;
        }
        else if (value == "<")
        {
            return rightValue < leftValue;
        }
        else if (value == "<=")
        {
            return rightValue <= leftValue;
        }
        else if (value == "|")
        {
            return rightValue || leftValue;
        }
        else if (value == "^")
        {
            return rightValue ^ leftValue;
        }
        else if (value == "&")
        {
            return rightValue && leftValue;
        }
        else if (value == "!=")
        {
            return rightValue != leftValue;
        }
    }
    else if (isVariable(value))
    {
        for (std::string var : ExpressionParser::knowsVariables)
        {
            if (var == value)
            {
                column += value.length() - 1;
                return ExpressionParser::variables[value];
            }
        }
        throw std::runtime_error("Runtime error: unknown identifier " + value);
    }
    else
    {
        // Assuming value is a number (possibly with commas)
        std::stringstream ss(value);
        double number;
        ss >> std::setprecision(15) >> number;

        if (ss.fail())
        {
            // if((value[0] >= 'a' && value[0] <= 'z') || (value[0] >= 'A' && value[0] <= 'Z') || value[0] == '_') {
            //     for(char c : value) {
            //         if(!(c >= 'a' && c <= 'z') && !(c >= 'A' && c <= 'Z') && c != '_' && !isdigit(c)) {
            //                 throw("Invalid operand: " + value);
            //         }

            //     }
            //}
            throw std::logic_error("Invalid number: " + value);
        }
        column += value.length() - 1;
        // std::cout << column << " is number " <<std::endl;
        return BooleanWrapper(number);
    }

    throw std::logic_error("Invalid operator: " + value);
}

void ExpressionNode::printInfix()
{
    column = 1;
    std::cout << strstrm.str() << std::endl;
    strstrm.str("");
    strstrm.clear();
}

void ExpressionNode::printResult()
{
    BooleanWrapper resultVar = computeResult();
    std::string result;
    if (resultVar.printType() == 'B')
    {
        result = resultVar.btos();
    }
    else if (resultVar.printType() == 'D')
    {
        result = resultVar.dtos();
        if (std::stod(result) == std::floor(std::stod(result)))
        {
            std::cout << std::floor(std::stod(result)) << std::endl;
        }
        else
        {
            // print with only enough precision to show the value (no trailing zeros)
            std::cout << std::stod(result) << std::endl;
        }
    }
}
