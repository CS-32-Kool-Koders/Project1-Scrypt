
#include "./lib/parser.h"
#include "./lib/lexer.h"
// g++ -std=c++17 parse.cpp ./lib/lexer.cpp -Wall -Wextra -Werror
// ./a.out

Parser::Parser(std::string token){
    this->token = token;
}

Parser::~Parser(){
    // for (Node* node: parseStack){
    //     delete node;
    // }
    while (!parseStack.empty()){
            Node * temp = parseStack.top();
            parseStack.pop();
            delete temp;
        }
}

Node::Node(std::string data){
    this->data = data;
    treeVec = std::vector<Node*>();
    for (Node* &node: treeVec){
        node = nullptr;
    }
}
Node::~Node(){
    for (Node* &node: treeVec){
        delete node;
    }
    treeVec.clear();
}

bool Parser::isOperator(std::string c){
    return (c == "+")|| (c == "-") || (c == "*") || (c == "/");
}


bool Parser::isNumber(std::string num) {
    int dec = 0;
    for (char c : num) {
        if (!isdigit(c) && c != '.') return false;
        if (c == '.') dec++;
    }
    return dec <= 1;
}

Node* Parser::parse(std::string token){
    if (token.empty()){
        exit(2);
    }

    std::string s;
    std::stringstream ss(token);
    std::vector<std::string> tokens;
    // int parenthesesCounter = 0; // To track opened and closed parentheses

    while (getline(ss, s, ' ')){
        tokens.push_back(s);
    }

    for (auto i: tokens){
        if (isOperator(std::string(i))){
            Node* oper = nullptr;
        try {
            oper = new Node(std::string(i));
            parseStack.push(oper);

        } catch(...) {
            delete oper;  
            oper = nullptr;
            throw;  
        }
        }
        else if (i == "("){
            // parenthesesCounter++;
            parseStack.push(nullptr);
        }
        else if (i == ")"){
            std::vector<Node*> tempVec;
            Node* temp = nullptr;
            while (!parseStack.empty() && parseStack.top() != nullptr){
                temp = parseStack.top(); 
                parseStack.pop();
                if (isOperator(temp->data)){
                    if (temp->treeVec.empty()){
                        temp->treeVec = tempVec;
                        continue;
                    } else {
                        
                    }
                }
                tempVec.push_back(temp);
            }
            //new code
            
            parseStack.pop();
            parseStack.push(temp);
            if (temp->treeVec != tempVec){
                for (auto i: tempVec){
                    delete i;
                }
            }
        }
        else if (isNumber(i)){
            Node* digit = new Node(std::string(i));
            parseStack.push(digit);
        }
    }

    // Mismatched parentheses
    // if (parenthesesCounter != 0) {
    //     std::cout << "Error: Mismatched parentheses." << std::endl;
    //     exit(2);
    // }

    if (parseStack.top()){
        return parseStack.top();
    }
    return nullptr;
}


void printTreeInfix(Node* node) {
    if (!node) return;
    if (node->treeVec.size() > 0) std::cout << "(";
    for (int i = node->treeVec.size() - 1; i >= 0; --i) {
        printTreeInfix(node->treeVec[i]);
        if (i != 0) std::cout << " " << node->data << " ";
    }
    if (node->treeVec.size() > 0) std::cout << ")";
    if (node->treeVec.empty()) {
        // Check for numbers that have .000 fractional part
        if (node->data.find('.') != std::string::npos) {
            double value = std::stod(node->data);
            if (floor(value) == value) {
                std::cout << static_cast<int>(value);
            } else {
                std::cout << node->data;
            }
        } else {
            std::cout << node->data;
        }
    }
}


double Parser::evaluate(Node* root) {
    if (root->treeVec.empty()) {
    if (root->data.size() == 1){
        if (isdigit(root->data[0])){
            return std::stod(std::string(1, root->data[0]));
        }
    } else{
        if (isNumber(root->data)) {
            return std::stod(std::string(root->data));
        }
    }
    }

    double result = evaluate(root->treeVec.back());  // Start from the last element
    for (int i = root->treeVec.size() - 2; i >= 0; --i) { // Traverse in reverse
        double operand = evaluate(root->treeVec[i]);
        if (root->data == "+") {
            result += operand;
        } else if (root->data == "-") {
            result -= operand;
        } else if (root->data == "*") {
            result *= operand;
        } else if (root->data == "/") {
            if (operand == 0) {
                std::cout << "Runtime error: division by zero." << std::endl;
                exit(3);
            }
            result /= operand;
        }
    }
    return result;
}


int main()
{
    auto reportUnexpectedToken = [](const Tokens& token) {
        std::cout << "Unexpected token at line " << token.line
                  << " column " << token.col << ": " << token.text << std::endl;
        exit(2);
    };

    std::string line; 
    int row = 0; 
    lexer Lexer;
    int new_line = 0; 
    while(!std::cin.eof()) {
        new_line += 1;
        if(getline(std::cin, line)) { 
            row += 1;
            Lexer.tokenize(row, line);
        }
    }
    if(new_line > row) {
        Lexer.tokenList.push_back(Tokens(new_line, 1, "END"));
    }
    else {
        Lexer.tokenList.push_back(Tokens(row, Lexer.tokenList.back().col, "END"));
    }

    int open = 0;
    int close = 0;
    std::string str;
    for (auto i : Lexer.tokenList){
        str += i.text + " ";
        if (i.text == "("){
            open++;
        }
        else if (i.text == ")"){
            close++;
        }
    }


if (open != close) {
    if (open > close) {
        // Unmatched opening parenthesis, point to END token
        Tokens adjustedEndToken = Lexer.tokenList.back();
        adjustedEndToken.col = Lexer.tokenList.back().col+2 ; // Adjust the column for END token
        reportUnexpectedToken(adjustedEndToken);
    } else {
        // Find the unmatched closing parenthesis
        int balance = 0;
        for (const auto& token : Lexer.tokenList) {
            if (token.text == "(") {
                balance++;
            } else if (token.text == ")") {
                balance--;
                if (balance < 0) {
                    reportUnexpectedToken(token);
                    break;
                }
            }
        }
    }
}



    if (str.empty() || str == "END ") {
        reportUnexpectedToken(Lexer.tokenList.back());
    }
    Parser parser(str);
    
 
    if (Lexer.tokenList.back().text != "END") {
        reportUnexpectedToken(Lexer.tokenList.back());
    }

    Node* root = parser.parse(str);
    
    if (root != nullptr) {
        // delete parser.parseStack.top();
        parser.parseStack.pop();
    }

    // Check for multiple top-level s-expressions
   
    bool insideParentheses = false;

    // bool topLevelParsed = false;

    for (auto i : Lexer.tokenList) {
        if (i.text == "(") {
            insideParentheses = true;
        } else if (i.text == ")") {
            insideParentheses = false;
            if (!insideParentheses) {
                // topLevelParsed = true;
            }
        } else if (parser.isOperator(i.text) && !insideParentheses) {
            reportUnexpectedToken(i);
            break;
        }
    }

    int numOperators = 0;
    int numNumbers = 0;
    int op = 0;
    int cl = 0; 
    for (size_t i = 0; i < Lexer.tokenList.size(); i++) {
        if (Lexer.tokenList[i].text == "(") {
            if (i == Lexer.tokenList.size() - 1 || 
                (Lexer.tokenList[i+1].text != "(" && !parser.isNumber(Lexer.tokenList[i+1].text) && !parser.isOperator(Lexer.tokenList[i+1].text))) {
                reportUnexpectedToken(Lexer.tokenList[i]);
            }
        }
        if (Lexer.tokenList[i].text == "("){
            op++;
        }
        if (Lexer.tokenList[i].text == ")"){
            cl++;
        }
        if (Lexer.tokenList[i].text == ")" && op == cl && Lexer.tokenList[i+1] .text!= "END"){
             std::cout << "Unexpected token at line " << Lexer.tokenList[i+1].line
                  << " column " << Lexer.tokenList[i+1].col << ": " <<Lexer.tokenList[i+1].text << std::endl;
            exit(2);
        }
        // if (topLevelParsed && i < Lexer.tokenList.size() - 1 && Lexer.tokenList[i + 1].text != "END") {
        //     reportUnexpectedToken(Lexer.tokenList[i + 1]);
        // }
         if (parser.isOperator(Lexer.tokenList[i].text)) {
            numOperators++;
        }
        else if (parser.isNumber(Lexer.tokenList[i].text)) {
            numNumbers++;
        }
        if (i == Lexer.tokenList.size()-1 && ((numNumbers == 0 && numOperators > 0) ||(numNumbers > 1 && numOperators == 0))) {
        std::cout << "Unexpected token at line " << Lexer.tokenList[i-1].line
                  << " column " << Lexer.tokenList[i-1].col << ": " <<Lexer.tokenList[i-1].text << std::endl;
        exit(2);
        }
        if (i == Lexer.tokenList.size() - 1) {
            if (!parser.parseStack.empty()) {
                std::cout << "Unexpected token at line " << Lexer.tokenList[i].line
                        << " column " << Lexer.tokenList[i].col << ": " << Lexer.tokenList[i].text << std::endl;
                exit(2);
            }
        }
        
        if (parser.isOperator(Lexer.tokenList[i].text) && i != 0){
            if(Lexer.tokenList[i-1].text != "("){
                std::cout << "Unexpected token at line " << Lexer.tokenList[i].line
                  << " column " << Lexer.tokenList[i].col << ": " <<Lexer.tokenList[i].text << std::endl;
            exit(2);
            }
        }
        if (parser.isOperator(Lexer.tokenList[i].text) && parser.isOperator(Lexer.tokenList[i+1].text)){
                std::cout << "Unexpected token at line " << Lexer.tokenList[i+1].line
                  << " column " << Lexer.tokenList[i+1].col << ": " <<Lexer.tokenList[i+1].text << std::endl;
            exit(2);
        }
        if (Lexer.tokenList[i].text == "(" && parser.isNumber(Lexer.tokenList[i+1].text)){
            std::cout << "Unexpected token at line " << Lexer.tokenList[i+1].line
                  << " column " << Lexer.tokenList[i+1].col << ": " <<Lexer.tokenList[i+1].text << std::endl;
            exit(2);
        }
    
        if (Lexer.tokenList[i].text == "(" &&
                parser.isOperator(Lexer.tokenList[i].text) &&
                Lexer.tokenList[i+2].text == ")") {
                reportUnexpectedToken(Lexer.tokenList[i]);
            }
    }
  
    // if (root == nullptr){
    //     exit(2);
    // }
    printTreeInfix(root);
    std::cout << std::endl << parser.evaluate(root) << std::endl;

    delete root;
    return 0;
}
