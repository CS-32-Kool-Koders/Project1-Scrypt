
#pragma once
#include <cmath>
#include <string>
#include <vector>
#include <iomanip>
#include <map>
#include <functional>

class Blocks;

struct Tokens;

struct BooleanWrapper
{
public:
    enum DataType
    {
        Undefined,
        Boolean,
        Double,
        Function,
    };

private:
    DataType type = DataType::Undefined;
    bool bvalue = false;
    double dvalue = 0;
    Blocks *block = nullptr;

public:
    BooleanWrapper()
    {
        this->type = DataType::Boolean;
        this->bvalue = false;
    }

    BooleanWrapper(bool value)
    {
        this->type = DataType::Boolean;
        this->bvalue = value;
    }

    BooleanWrapper(double value)
    {
        this->type = DataType::Double;
        this->dvalue = value;
    }

    BooleanWrapper(Blocks *value)
    {
        this->type = DataType::Function;
        this->block = value;
    }

    BooleanWrapper(std::string value)
    {
        if (value == "true")
        {
            this->type = DataType::Boolean;
            this->bvalue = true;
        }
        else if (value == "false")
        {
            this->type = DataType::Boolean;
            this->bvalue = false;
        }
        else if (std::stod(value))
        {
            this->type = DataType::Double;
            this->dvalue = std::stod(value);
        }
        else
            throw std::logic_error("Invalid boolean value");
    }

    // Maybe required ?
    // ~BooleanWrapper()
    // {
    //     if (block != nullptr)
    //     {
    //         delete block;
    //     }
    // }

    BooleanWrapper::DataType getType()
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
        if (type == DataType::Boolean)
        {
            return bvalue;
        }
        throw std::runtime_error("Runtime error: condition is not a bool.");
    }

    double getDvalue()
    {
        if (type == DataType::Double)
        {
            return dvalue;
        }
        throw std::runtime_error("Runtime error: condition is not a bool.");
    }

    std::string btos()
    {
        if (type == DataType::Boolean)
        {
            return this->bvalue ? "true" : "false";
        }
        throw std::runtime_error("Runtime error: condition is not a bool.");
    }

    std::string dtos()
    {
        if (type == DataType::Double)
        {
            return std::to_string(this->dvalue);
        }
        else
        {
            throw std::runtime_error("Runtime error: condition is not a bool.");
        }
    }

    // operator overloading
    // assignment auto type conversion
    BooleanWrapper operator=(bool value)
    {
        this->type = DataType::Boolean;
        this->bvalue = value;
        return *this;
    }

    BooleanWrapper operator=(double value)
    {
        this->type = DataType::Double;
        this->dvalue = value;
        return *this;
    }

    BooleanWrapper operator>(BooleanWrapper other)
    {
        if (type == DataType::Double && other.type == DataType::Double)
        {
            return BooleanWrapper((this->dvalue > other.dvalue));
        }
        else
        {
            throw std::runtime_error("Runtime error: invalid operand type.");
        }
    }

    BooleanWrapper operator>=(BooleanWrapper other)
    {
        if (type == DataType::Double && other.type == DataType::Double)
        {
            return BooleanWrapper((this->dvalue >= other.dvalue));
        }
        else
        {
            throw std::runtime_error("Runtime error: invalid operand type.");
        }
    }

    BooleanWrapper operator<(BooleanWrapper other)
    {
        if (type == DataType::Double && other.type == DataType::Double)
        {
            return BooleanWrapper((this->dvalue < other.dvalue));
        }
        else
        {
            throw std::runtime_error("Runtime error: invalid operand type.");
        }
    }

    BooleanWrapper operator<=(BooleanWrapper other)
    {
        if (type == DataType::Double && other.type == DataType::Double)
        {
            return BooleanWrapper((this->dvalue <= other.dvalue));
        }
        else
        {
            throw std::runtime_error("Runtime error: invalid operand type.");
        }
    }

    BooleanWrapper operator^(BooleanWrapper other)
    {
        if (type == DataType::Boolean && other.type == DataType::Boolean)
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
        if (type == DataType::Boolean && other.type == DataType::Boolean)
        {
            return BooleanWrapper(this->bvalue == other.bvalue);
        }
        else if (type == DataType::Double && other.type == DataType::Double)
        {
            return BooleanWrapper(this->dvalue == other.dvalue);
        }
        else
        {
            return BooleanWrapper(false);
        }
    }

    BooleanWrapper operator!=(BooleanWrapper other)
    {
        if (type == DataType::Boolean && other.type == DataType::Boolean)
        {
            return BooleanWrapper(this->bvalue != other.bvalue);
        }
        else if (type == DataType::Double && other.type == DataType::Double)
        {
            return BooleanWrapper(this->dvalue != other.dvalue);
        }
        else
        {
            return BooleanWrapper(false);
        }
    }

    BooleanWrapper operator&&(BooleanWrapper other)
    {
        if (type == DataType::Boolean && other.type == DataType::Boolean)
        {
            return BooleanWrapper(this->bvalue && other.bvalue);
        }
        // else if (type == DataType::Double && other.type == DataType::Double)
        // {
        //     return BooleanWrapper(this->dvalue && other.dvalue);
        // }
        throw std::runtime_error("Runtime error: invalid operand type.");
    }

    BooleanWrapper operator||(BooleanWrapper other)
    {
        if (type == DataType::Boolean && other.type == DataType::Boolean)
        {
            return BooleanWrapper(this->bvalue || other.bvalue);
        }
        // else if (type == DataType::Double && other.type == DataType::Double)
        // {
        //     return BooleanWrapper(this->dvalue || other.dvalue);
        // }
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
        if (type == DataType::Double && other.type == DataType::Double)
        {
            return dvalue + other.dvalue;
        }
        else
        {
            throw std::runtime_error("Runtime error: invalid operand type.");
        }
    }

    BooleanWrapper operator-(BooleanWrapper other)
    {
        if (type == DataType::Double && other.type == DataType::Double)
        {
            return dvalue - other.dvalue;
        }
        else
        {
            throw std::runtime_error("Runtime error: invalid operand type.");
        }
    }

    BooleanWrapper operator*(BooleanWrapper other)
    {
        if (type == DataType::Double && other.type == DataType::Double)
        {
            return dvalue * other.dvalue;
        }
        else
        {
            throw std::runtime_error("Runtime error: invalid operand type.");
        }
    }

    BooleanWrapper operator/(BooleanWrapper other)
    {
        if (type == DataType::Double && other.type == DataType::Double)
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
            throw std::runtime_error("Runtime error: invalid operand type.");
        }
    }

    BooleanWrapper operator%(BooleanWrapper other)
    {
        if (type == DataType::Double && other.type == DataType::Double)
        {
            return fmod(dvalue, other.dvalue);
        }
        else
        {
            throw std::runtime_error("Runtime error: invalid operand type.");
        }
    }

    // BooleanWrapper operator+=(double num)
    // {
    //     if (type == DataType::Double && other.type == DataType::Double)
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
        if (bw.type == DataType::Double)
        {
            os << bw.dvalue;
            return os;
        }
        else if (bw.type == DataType::Boolean)
        {
            os << bw.bvalue;
            return os;
        }
        else
        {
            throw std::runtime_error("Runtime error: invalid operand type.");
        }
    }

    friend std::istream &operator>>(std::istream &is, BooleanWrapper &bw)
    {
        if (bw.type == DataType::Double)
        {
            is >> bw.dvalue;
            return is;
        }
        else if (bw.type == DataType::Boolean)
        {
            is >> bw.bvalue;
            return is;
        }
        else
        {
            throw std::runtime_error("Runtime error: invalid operand type.");
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
    void printInfix(bool newLine = true);
    void printTree();
    void printResult(BooleanWrapper);
    void checkParentheses(std::string tokenString);
};

class ExpressionParser
{
public:
    static std::vector<std::string> knowsVariables;
    static std::map<std::string, BooleanWrapper> variables;
    static std::string line;
    std::vector<Tokens> tokens;
    std::size_t currentIndex;

private:
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
    std::vector<Tokens> getTokens()
    {
        return tokens;
    }
    ExpressionNode *
    parseExpression();

private:
    /*
        Parentheses (( and )).
        Multiplication, division, and modulo (*, /, and %).
        Addition and subtraction (+ and -).
        Ordered comparison (<, <=, >, and >=).
        Equality and inequality (== and !=).
        Logical and (&).
        Logical exclusive or (^).
        Logical inclusive or (|).
        Assignment (=).
    */
    ExpressionNode *parseAssignment();          // =
    ExpressionNode *parseLogicalOr();           // |
    ExpressionNode *parseLogicalXor();          // ^
    ExpressionNode *parseLogicalAnd();          // &
    ExpressionNode *parseEquality();            // ==, !=
    ExpressionNode *parseComparison();          // <, <=, >, >=
    ExpressionNode *parseAdditionSubtraction(); // +, -
    ExpressionNode *parseMultiplyDivide();      // *, /, %
    ExpressionNode *parseOperand();
    ExpressionNode *parseOperator(std::function<ExpressionNode *()> parseFunction, std::vector<std::string> operators);
};