#pragma once
#include <cmath>
#include <string>
#include <vector>
#include <iomanip>
#include <map>
#include <functional>

struct Tokens;

struct BooleanWrapper
{
private:
    char type = '\0';
    bool bvalue = false;
    double dvalue = 0;

public:
    BooleanWrapper(bool value)
    {
        this->type = 'B';
        this->bvalue = value;
    }

    BooleanWrapper(double value)
    {
        this->type = 'D';
        this->dvalue = value;
    }

    BooleanWrapper(std::string value)
    {
        if (value == "true")
        {
            this->type = 'B';
            this->bvalue = true;
        }
        else if (value == "false")
        {
            this->type = 'B';
            this->bvalue = false;
        }
        else if (std::stod(value))
        {
            this->type = 'D';
            this->dvalue = std::stod(value);
        }
        else
            throw std::logic_error("Invalid boolean value");
    }

    char printType()
    {
        return type;
    }
    static bool isBoolean(std::string value)
    {
        if (value == "true" || value == "false")
            return true;
        else
            return false;
    }

    bool getBvalue()
    {
        if (type == 'B')
        {
            return bvalue;
        }
        throw std::runtime_error("wrong lol");
    }

    double getDvalue()
    {
        if (type == 'D')
        {
            return dvalue;
        }
        throw std::runtime_error("wrong lol");
    }

    std::string btos()
    {
        if (type == 'B')
        {
            return this->bvalue ? "true" : "false";
        }
        throw std::runtime_error("wrong lol");
    }

    std::string dtos()
    {
        if (type == 'D')
        {
            return std::to_string(this->dvalue);
        }
        else
        {
            throw std::runtime_error("wrong lol");
        }
    }

    // operator overloading
    BooleanWrapper operator>(BooleanWrapper other)
    {
        if (type == 'D' && other.type == 'D')
        {
            return BooleanWrapper(this->bvalue > other.bvalue);
        }
        else
        {
            throw std::runtime_error("Runtime error: invalid operand type.");
        }
    }
    BooleanWrapper operator>=(BooleanWrapper other)
    {
        if (type == 'D' && other.type == 'D')
        {
            return BooleanWrapper(this->bvalue >= other.bvalue);
        }
        else
        {
            throw std::runtime_error("Runtime error: invalid operand type.");
        }
    }
    BooleanWrapper operator<(BooleanWrapper other)
    {
        if (type == 'D' && other.type == 'D')
        {
            return BooleanWrapper(this->bvalue < other.bvalue);
        }
        else
        {
            throw std::runtime_error("Runtime error: invalid operand type.");
        }
    }
    BooleanWrapper operator<=(BooleanWrapper other)
    {
        if (type == 'B' && other.type == 'B')
        {
            return BooleanWrapper(this->bvalue <= other.bvalue);
        }
        else
        {
            throw std::runtime_error("Runtime error: invalid operand type.");
        }
    }
    BooleanWrapper operator^(BooleanWrapper other)
    {
        if (type == 'B' && other.type == 'B')
        {
            return BooleanWrapper(!this->bvalue != !other.bvalue);
        }
        else
        {
            throw std::runtime_error("Runtime error: invalid operand type.");
        }
    }
    BooleanWrapper operator==(BooleanWrapper other)
    {
        // std::cout << "wow equal equal" << std::endl;
        if (type == 'B' && other.type == 'B')
        {
            return BooleanWrapper(this->bvalue == other.bvalue);
        }
        else if (type == 'D' && other.type == 'D')
        {
            return BooleanWrapper(this->dvalue == other.dvalue);
        }
        else
        {
            throw std::runtime_error("Runtime error: invalid operand type.");
        }
    }

    BooleanWrapper operator!=(BooleanWrapper other)
    {
        if (type == 'B' && other.type == 'B')
        {
            return BooleanWrapper(this->bvalue != other.bvalue);
        }
        else if (type == 'D' && other.type == 'D')
        {
            return BooleanWrapper(this->dvalue != other.dvalue);
        }
        else
        {
            throw std::runtime_error("Runtime error: invalid operand type.");
        }
    }

    BooleanWrapper operator&&(BooleanWrapper other)
    {
        if (bvalue && other.bvalue)
        {
            return BooleanWrapper(this->bvalue && other.bvalue);
        }
        else if (dvalue && other.dvalue)
        {
            return BooleanWrapper(this->dvalue && other.dvalue);
        }
        throw std::runtime_error("Runtime error: invalid operand type.");
    }

    BooleanWrapper operator||(BooleanWrapper other)
    {
        if (bvalue && other.bvalue)
        {
            return BooleanWrapper(this->bvalue || other.bvalue);
        }
        else if (dvalue && other.dvalue)
        {
            return BooleanWrapper(this->dvalue || other.dvalue);
        }
        throw std::runtime_error("Runtime error: invalid operand type.");
    }

    BooleanWrapper operator!()
    {
        if (bvalue)
        {
            return BooleanWrapper(!this->bvalue);
        }
        throw std::runtime_error("Runtime error: invalid operand type.");
    }

    BooleanWrapper operator+(BooleanWrapper other)
    {
        if (type == 'D' && other.type == 'D')
        {
            return dvalue + other.dvalue;
        }
        else
        {
            throw std::runtime_error("Runtime error: Invalid operand type.");
        }
    }

    BooleanWrapper operator-(BooleanWrapper other)
    {
        if (type == 'D' && other.type == 'D')
        {
            return dvalue - other.dvalue;
        }
        else
        {
            throw std::runtime_error("Runtime error: Invalid operand type.");
        }
    }

    BooleanWrapper operator*(BooleanWrapper other)
    {
        if (type == 'D' && other.type == 'D')
        {
            return dvalue * other.dvalue;
        }
        else
        {
            throw std::runtime_error("Runtime error: Invalid operand type.");
        }
    }

    BooleanWrapper operator/(BooleanWrapper other)
    {
        if (type == 'D' && other.type == 'D')
        {
            if (other.dvalue == 0)
            {
                throw std::runtime_error("Runtime error: division by zero.");
            }
            else
            {
                return dvalue / other.dvalue;
            }
        }
        else
        {
            throw std::runtime_error("Runtime error: Invalid operand type.");
        }
    }

    BooleanWrapper operator%(BooleanWrapper other)
    {
        if (type == 'D' && other.type == 'D')
        {
            return fmod(dvalue, other.dvalue);
        }
        else
        {
            throw std::runtime_error("Runtime error: Invalid operand type.");
        }
    }

    // BooleanWrapper operator+=(double num)
    // {
    //     if (type == 'D' && other.type == 'D')
    //     {
    //         return dvalue + num;
    //     }
    //     else
    //     {
    //         throw std::runtime_error("Runtime error: Invalid operand type.");
    //     }
    // }

    // BooleanWrapper operator-=(BooleanWrapper)
    // {
    //     throw std::runtime_error("Runtime error: Invalid operand type.");
    // }

    // BooleanWrapper operator*=(BooleanWrapper)
    // {
    //     throw std::runtime_error("Runtime error: Invalid operand type.");
    // }

    // BooleanWrapper operator/=(BooleanWrapper)
    // {
    //     throw std::runtime_error("Runtime error: Invalid operand type.");
    // }

    // BooleanWrapper operator%=(BooleanWrapper)
    // {
    //     throw std::runtime_error("Runtime error: Invalid operand type.");
    // }

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
        if (bw.type == 'D')
        {
            os << bw.dvalue;
            return os;
        }
        else if (bw.type == 'B')
        {
            os << bw.bvalue;
            return os;
        }
        else
        {
            throw std::runtime_error("Runtime error: Invalid operand type.");
        }
    }

    friend std::istream &operator>>(std::istream &is, BooleanWrapper &bw)
    {
        if (bw.type == 'D')
        {
            is >> bw.dvalue;
            return is;
        }
        else if (bw.type == 'B')
        {
            is >> bw.bvalue;
            return is;
        }
        else
        {
            throw std::runtime_error("Runtime error: Invalid operand type.");
        }
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

    BooleanWrapper computeResult();
    bool isVariable(std::string value);
    void getVariablesNames();
    void computeInfix();
    void printInfix();
    void printTree();
    void printResult();
    void checkParentheses(std::string tokenString);
};

class ExpressionParser
{
public:
    static std::vector<std::string> knowsVariables;
    static std::map<std::string, bool> boolVariables;
    static std::map<std::string, double> variables;
    static std::string line;

private:
    std::vector<Tokens> tokens;
    std::size_t currentIndex;
    // std::map<std::function<ExpressionNode *()>, std::string> operatorsFn = {
    //     {std::bind(&ExpressionParser::parseMultiplyDivide, this), std::vector<std::string>{"+", "-"}},
    //     {std::bind(&ExpressionParser::parseOperand, this), std::vector<std::string>{"*", "/"}},
    // };

public:
    ExpressionParser(std::vector<Tokens> tokens)
    {
        this->tokens = tokens;
        this->currentIndex = 0;
    }

    ExpressionNode *
    parseExpression();

private:
    ExpressionNode *parseAssignment();
    ExpressionNode *parseAddSubtract();
    ExpressionNode *parseMultiplyDivide();
    ExpressionNode *parseComparison();
    ExpressionNode *parseOperand();
    ExpressionNode *parseOperator(std::function<ExpressionNode *()> parseFunction, std::vector<std::string> operators);
};
