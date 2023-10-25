#include <iostream>
#include <vector>
#include <cmath>
#include "infix_parser.h"
#include "tokens.h"

void ExpressionNode::printInfix()
{
    if (left != nullptr && right != nullptr)
    {
        std::cout << "(";
        left->printInfix();
        if (value == "+" || value == "-" || value == "*" || value == "/")
        {
            std::cout << " " << value << " ";
        }
        else
        {
            std::cout << value;
        }
        right->printInfix();
        std::cout << ")";
    }
    else
    {
        std::cout << value;
    }
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
            knowsVariables.push_back(left->value);
        }
        else
        {
            throw std::runtime_error("Invalid variable name");
        }
    }

    right->getVariablesNames();
}

double ExpressionNode::computeResult()
{
    if (value == "+" || value == "-" || value == "*" || value == "/" || value == "=")
    {
        if (left == nullptr || right == nullptr)
        {
            throw std::runtime_error("Invalid expression");
        }
        double leftValue = left->computeResult();
        double rightValue = right->computeResult();

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
            return leftValue * rightValue;
        }
        else if (value == "/")
        {
            if (rightValue == 0)
            {
                throw std::runtime_error("Division by zero");
            }
            return leftValue / rightValue;
        }
        else if (value == "=")
        {
            variables[left->value] = rightValue;
            return rightValue;
        }
    }
    else if (isVariable(value))
    {
        for (std::string var : knowsVariables)
        {
            if (var == value)
            {
                return variables[value];
            }
        }
        throw std::runtime_error("Unknown variable: " + value);
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
            throw std::runtime_error("Invalid number: " + value);
        }
        return number;
    }

    throw std::runtime_error("Invalid operator: " + value);
}

void ExpressionNode::printResult()
{
    getVariablesNames();
    double result = computeResult();
    knowsVariables.clear();
    // if the number is an integer, print it without a decimal point
    if (result == std::floor(result))
    {
        std::cout << std::floor(result);
    }
    else
    {
        // print with only enough precision to show the value (no trailing zeros)
        std::cout << result; // not sure if std::precision is needed for that case
    }
}