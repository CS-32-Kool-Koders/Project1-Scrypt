#pragma once

#include <string>
#include <vector>
#include <iomanip>
#include <map>

struct Tokens;

struct BooleanWrapper
{
private:
    bool value;

public:
    BooleanWrapper(bool value)
    {
        this->value = value;
    }

    BooleanWrapper(std::string value)
    {
        if (value == "true")
            this->value = true;
        else if (value == "false")
            this->value = false;
        else
            throw std::logic_error("Invalid boolean value");
    }

    static bool isBoolean(std::string value)
    {
        if (value == "true" || value == "false")
            return true;
        else
            return false;
    }

    std::string getAsString()
    {
        return this->value ? "true" : "false";
    }

    // operator overloading
    BooleanWrapper operator==(BooleanWrapper other)
    {
        return BooleanWrapper(this->value == other.value);
    }

    BooleanWrapper operator!=(BooleanWrapper other)
    {
        return BooleanWrapper(this->value != other.value);
    }

    BooleanWrapper operator&&(BooleanWrapper other)
    {
        return BooleanWrapper(this->value && other.value);
    }

    BooleanWrapper operator||(BooleanWrapper other)
    {
        return BooleanWrapper(this->value || other.value);
    }

    BooleanWrapper operator!()
    {
        return BooleanWrapper(!this->value);
    }

    BooleanWrapper operator+(BooleanWrapper)
    {
        throw std::runtime_error("Runtime error: Invalid operand type.");
    }

    BooleanWrapper operator-(BooleanWrapper)
    {
        throw std::runtime_error("Runtime error: Invalid operand type.");
    }

    BooleanWrapper operator*(BooleanWrapper)
    {
        throw std::runtime_error("Runtime error: Invalid operand type.");
    }

    BooleanWrapper operator/(BooleanWrapper)
    {
        throw std::runtime_error("Runtime error: Invalid operand type.");
    }

    BooleanWrapper operator%(BooleanWrapper)
    {
        throw std::runtime_error("Runtime error: Invalid operand type.");
    }

    BooleanWrapper operator+=(BooleanWrapper)
    {
        throw std::runtime_error("Runtime error: Invalid operand type.");
    }

    BooleanWrapper operator-=(BooleanWrapper)
    {
        throw std::runtime_error("Runtime error: Invalid operand type.");
    }

    BooleanWrapper operator*=(BooleanWrapper)
    {
        throw std::runtime_error("Runtime error: Invalid operand type.");
    }

    BooleanWrapper operator/=(BooleanWrapper)
    {
        throw std::runtime_error("Runtime error: Invalid operand type.");
    }

    BooleanWrapper operator%=(BooleanWrapper)
    {
        throw std::runtime_error("Runtime error: Invalid operand type.");
    }

    BooleanWrapper operator++()
    {
        throw std::logic_error("Invalid operation with boolean");
    }

    BooleanWrapper operator--()
    {
        throw std::logic_error("Invalid operation with boolean");
    }

    friend std::ostream &operator<<(std::ostream &os, const BooleanWrapper &bw)
    {
        os << bw.value;
        return os;
    }

    friend std::istream &operator>>(std::istream &is, BooleanWrapper &bw)
    {
        is >> bw.value;
        return is;
    }
};

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
    void printTree();
    void printResult();
    void checkParentheses(std::string tokenString);
};

class ExpressionParser
{
public:
    static std::vector<std::string> knowsVariables;
    static std::map<std::string, double> variables;
    static std::string line;

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
