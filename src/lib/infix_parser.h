#pragma once

#include <string>
#include <vector>
#include <iomanip>
#include <map>

static std::vector<std::string> knowsVariables = {};
static std::map<std::string, double> variables = {};

struct Tokens;

class ExpressionNode
{
public:
    std::string value;
    ExpressionNode *left;
    ExpressionNode *right;

    ExpressionNode(std::string value)
    {
        this->value = value;
        this->left = nullptr;
        this->right = nullptr;
    }

    ~ExpressionNode()
    {
        if (left != nullptr)
        {
            delete left;
        }
        if (right != nullptr)
        {
            delete right;
        }
    }

    double computeResult();
    bool isVariable(std::string value);
    void getVariablesNames();
    void printInfix();
    void printResult();
};

class ExpressionParser
{
private:
    std::vector<Tokens> tokens;
    std::size_t currentIndex;

public:
    ExpressionParser(std::vector<Tokens> tokens)
    {
        this->tokens = tokens;
        this->currentIndex = 0;
    }

    ExpressionNode *parseExpression();

private:
    ExpressionNode *parseAssignment();
    ExpressionNode *parseAddSubtract();
    ExpressionNode *parseMultiplyDivide();
    ExpressionNode *parseOperand();
};
