#include <iostream>
#include <stack>
#include <vector>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
// #include "./tokens.h"
#include <algorithm>
#include <string>
#include <sstream>
#include <cmath>
#include <unordered_map>
#include <unordered_set>

//#include "lexer.h"

// #include "./lib/lexer.h"

struct Node {
    std::string data;
    std::vector<Node*> treeVec;
    // bool id_child = false;
    Node(std::string data);
    ~Node();
};

struct Parser {
    //implement stack of nodes, with pointers to children if the stack is an operator
    // and no pointers if an operand
    std::unordered_set<std::string> help;
    std::unordered_map<std::string, double> vars;
    std::stack<Node*> parseStack; 
    std::string token;
    Parser();
    Node* parse(std::string tokenStr);
    ~Parser();
    bool isOperator(std::string c);
    void printTreeInfix(Node* node);
    double evaluate(Node* root, std::unordered_map<std::string, double> &variables);
    bool isNumber(std::string num);
    bool isIdentifier(std::string i);
    // void makeSet(Node* root);
    // void errorHelper(std::vector<Tokens> tokenList);
    // auto reportUnexpectedToken = [](const Tokens& token)
};