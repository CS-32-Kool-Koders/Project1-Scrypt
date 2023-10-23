#include <iostream>
#include <vector>
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
