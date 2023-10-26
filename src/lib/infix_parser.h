#pragma once

#include <string>
#include <vector>
#include <iomanip>
#include <map>

struct Tokens;

class ExpressionNode
{
public:
    std::string value;
    ExpressionNode *left;
    ExpressionNode *right;

    //Josh added lineNumber and colNumber to the data of the ExpressionNode object
    int lineNumber;
    int colNumber;

    ExpressionNode(std::string value)
    {
        this->value = value;
        this->left = nullptr;
        this->right = nullptr;
        this->lineNumber = lineNumber;
        this->colNumber = colNumber;
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
    void printTree();
    void printResult();
};

class ExpressionParser
{
public:
    static std::vector<std::string> knowsVariables;
    static std::map<std::string, double> variables;

private:
    std::vector<Tokens> tokens;
    std::size_t currentIndex;
    //Josh added currentLine initialization
    std::size_t currentLine;

public:
    ExpressionParser(std::vector<Tokens> tokens)
    {
        this->tokens = tokens;
        this->currentIndex = 0;
        //Josh added currentLine and initialized to 0 
        this->currentLine = 0;
    }

    ExpressionNode *parseExpression();

private:
    ExpressionNode *parseAssignment();
    ExpressionNode *parseAddSubtract();
    ExpressionNode *parseMultiplyDivide();
    ExpressionNode *parseOperand();
};
