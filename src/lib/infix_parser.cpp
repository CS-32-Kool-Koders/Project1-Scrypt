#include <iostream>
#include <vector>
#include <cmath>
#include "infix_parser.h"
#include "tokens.h"

std::vector<std::string> ExpressionParser::knowsVariables;
std::map<std::string, double> ExpressionParser::variables;
double result;
std::stringstream strstrm;
void ExpressionNode::printInfix()
{
    if (left != nullptr && right != nullptr)
    {
        strstrm << "(";
        left->printInfix();
        if (value == "+" || value == "-" || value == "*" || value == "/" || value == "=")
        {
            strstrm << " " << value << " ";
        }
        else
        {
            strstrm << value;
        }
        right->printInfix();
        strstrm << ")";
    }
    else
    {
        strstrm << value;
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
        ExpressionNode *node = new ExpressionNode(tokens[currentIndex].text);
        currentIndex++;
        node->left = left;
        node->right = parseAssignment();
        return node;
    }
    return left;
}

ExpressionNode *ExpressionParser::parseAddSubtract()
{
    ExpressionNode *left = parseMultiplyDivide();
    while (currentIndex < tokens.size() &&
           (tokens[currentIndex].text == "+" || tokens[currentIndex].text == "-"))
    {
        std::string op = tokens[currentIndex].text;
        currentIndex++;
        ExpressionNode *right = parseMultiplyDivide();
        ExpressionNode *node = new ExpressionNode(op);
        node->left = left;
        node->right = right;
        left = node;
        delete node;
    }
    return left;
}

ExpressionNode *ExpressionParser::parseMultiplyDivide()
{
    ExpressionNode *left = parseOperand();
    while (currentIndex < tokens.size() &&
           (tokens[currentIndex].text == "*" || tokens[currentIndex].text == "/"))
    {
        std::string op = tokens[currentIndex].text;
        currentIndex++;
        ExpressionNode *right = parseOperand();
        ExpressionNode *node = new ExpressionNode(op);
        node->left = left;
        node->right = right;
        left = node;
        delete node;
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
    if (!isdigit(var[0]))
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
        if (isVariable(left->value))
        {
            ExpressionParser::knowsVariables.push_back(left->value);
        }
        else
        {
            throw std::logic_error("Invalid variable name");
        }
    }

    right->getVariablesNames();
}
int column = 1;
double ExpressionNode::computeResult()
{
    if (value == "+" || value == "-" || value == "*" || value == "/" || value == "=" || value == "END")
    {
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
        double leftValue = left->computeResult();
        column += 4;
        double rightValue = right->computeResult();
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
            if (rightValue == 0)
            {
                throw std::runtime_error("Runtime error: division by zero.");
            }
            return leftValue / rightValue;
        }
        else if (value == "=")
        {
            ExpressionParser::variables[left->value] = rightValue;
            return rightValue;
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
        return number;
    }

    throw std::logic_error("Invalid operator: " + value);
}

void ExpressionNode::printResult()
{
    // getVariablesNames();
    column = 1;
    // double result = computeResult();
    // knowsVariables.clear();
    // if the number is an integer, print it without a decimal point
    // if (result == std::floor(result))
    // {
    //     std::cout << std::floor(result);
    // }
    // else
    // {
    //     // print with only enough precision to show the value (no trailing zeros)
    //     std::cout << result; // not sure if std::precision is needed for that case
    // }
    std::cout << strstrm.str() << std::endl;
    // std::cout << computeResult() << std::endl;
    strstrm.str("");
    strstrm.clear();
}
