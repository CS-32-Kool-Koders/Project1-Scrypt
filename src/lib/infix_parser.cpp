#include <iostream>
#include <vector>
#include "infix_parser.h"
#include "tokens.h"
#include <algorithm>

std::vector<std::string> supportedOperators = {
    "+", "-", "*", "/", "=", "END", "%"};

std::vector<std::string> supportedComparators = {
    "==", ">", ">=", "<", "<=", "|", "^", "&", "!="};

std::vector<std::string> supportedOpAndCmpWithEnd = {
    "+", "-", "*", "/", "=", "END", "%", "==", ">", ">=", "<", "<=", "|", "^", "&", "!="};

std::vector<std::string> supportedOpAndCmpWithoutEnd = {
    "+", "-", "*", "/", "=", "%", "==", ">", ">=", "<", "<=", "|", "^", "&", "!="};

std::vector<std::string> ExpressionParser::knowsVariables;
std::map<std::string, BooleanWrapper> ExpressionParser::variables;
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
        if (std::find(supportedOpAndCmpWithoutEnd.begin(), supportedOpAndCmpWithoutEnd.end(), value) != supportedOpAndCmpWithoutEnd.end())
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
        if (std::find(supportedOpAndCmpWithoutEnd.begin(), supportedOpAndCmpWithoutEnd.end(), value) != supportedOpAndCmpWithoutEnd.end())
        {
            strstrm << value;
        }
        else if (!isVariable(value))
        {
            if (BooleanWrapper::isBoolean(value))
            {
                strstrm << value;
            }
            else if (value == "print" || value == "if" || value == "while" || value == "else")
            {
                strstrm << value;
            }
            else if (value == "{" || value == "}" || value == "(" || value == ")")
            {
                strstrm << value;
            }
            else if (std::floor(std::stod(value)) == std::stod(value))
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
                strstrm.str("");
                strstrm.clear();
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
            strstrm.str("");
            strstrm.clear();
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
            strstrm.str("");
            strstrm.clear();
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

// ExpressionNode *parseAssignment(); // =
// ExpressionNode *parseLogicalOr(); // |
// ExpressionNode *parseLogicalXor(); // ^
// ExpressionNode *parseLogicalAnd(); // &
// ExpressionNode *parseEquality(); // ==, !=
// ExpressionNode *parseComparison(); // <, <=, >, >=
// ExpressionNode *parseAdditionSubtraction(); // +, -
// ExpressionNode *parseMultiplyDivide(); // *, /, %

ExpressionNode *ExpressionParser::parseAssignment()
{
    ExpressionNode *left = parseLogicalOr();
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

ExpressionNode *ExpressionParser::parseLogicalOr()
{
    return parseOperator(std::bind(&ExpressionParser::parseLogicalXor, this), {"|"});
}

ExpressionNode *ExpressionParser::parseLogicalXor()
{
    return parseOperator(std::bind(&ExpressionParser::parseLogicalAnd, this), {"^"});
}

ExpressionNode *ExpressionParser::parseLogicalAnd()
{
    return parseOperator(std::bind(&ExpressionParser::parseEquality, this), {"&"});
}

ExpressionNode *ExpressionParser::parseEquality()
{
    return parseOperator(std::bind(&ExpressionParser::parseComparison, this), {"==", "!="});
}

ExpressionNode *ExpressionParser::parseComparison()
{
    return parseOperator(std::bind(&ExpressionParser::parseAdditionSubtraction, this), {"<", "<=", ">", ">="});
}

ExpressionNode *ExpressionParser::parseAdditionSubtraction()
{
    return parseOperator(std::bind(&ExpressionParser::parseMultiplyDivide, this), {"+", "-"});
}

ExpressionNode *ExpressionParser::parseMultiplyDivide()
{
    return parseOperator(std::bind(&ExpressionParser::parseOperand, this), {"*", "/", "%"});
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
    if (var == "true" || var == "false" || var == "print" || var == "if" || var == "while" || var == "else")
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
            strstrm.str("");
            strstrm.clear();
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
    if (std::find(supportedOpAndCmpWithEnd.begin(), supportedOpAndCmpWithEnd.end(), value) != supportedOpAndCmpWithEnd.end())
    {
        if (value == "=" && right != nullptr && right->value == "END")
        {
            int endColumn = ExpressionParser::line.find("END");
            strstrm.str("");
            strstrm.clear();
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
            strstrm.str("");
            strstrm.clear();
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
            ExpressionParser::variables[left->value] = rightValue;
            return rightValue;
        }
        else if (value == "%")
        {
            return leftValue % rightValue;
        }
        else if (value == "==")
        {
            return leftValue == rightValue;
        }
        else if (value == ">")
        {
            return leftValue > rightValue;
        }
        else if (value == ">=")
        {
            return leftValue >= rightValue;
        }
        else if (value == "<")
        {
            return leftValue < rightValue;
        }
        else if (value == "<=")
        {
            return leftValue <= rightValue;
        }
        else if (value == "|")
        {
            return leftValue || rightValue;
        }
        else if (value == "^")
        {
            return leftValue ^ rightValue;
        }
        else if (value == "&")
        {
            return leftValue && rightValue;
        }
        else if (value == "!=")
        {
            return leftValue != rightValue;
        }
        //added for scrypt shit
        // else if (value == "}"){
        //     return true;
        // }
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

        // a non initialized variable is false
        return BooleanWrapper(false);
    }
    else if (!BooleanWrapper::isBoolean(value))
    {
        if (value == "print" || value == "if" || value == "while" || value == "else")
        {
            strstrm.str("");
            strstrm.clear();
            std::string throw_message = "Unexpected token at line 1 column " + std::to_string(column) + ": " + value;
            column = 1;
            throw std::logic_error(throw_message);
        }

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
            // if (value != "}"){
                throw std::logic_error("Invalid number: " + value);
            // } else {
            //     std::cout<<"";
            // }
            
        }
        column += value.length() - 1;
        // std::cout << column << " is number " <<std::endl;
        return BooleanWrapper(number);
    }
    else
    {
        column += value.length() - 1;
        return BooleanWrapper(value);
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

void ExpressionNode::printResult(BooleanWrapper resultVar)
{
    // BooleanWrapper resultVar = computeResult();
    if (resultVar.printType() == 'B')
    {
        std::cout << resultVar.btos() << std::endl;
    }
    else if (resultVar.printType() == 'D')
    {
        std::string result = resultVar.dtos();
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