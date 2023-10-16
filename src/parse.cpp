#include "./lib/parser.h"
#include "./lib/lexer.h"
// g++ -std=c++17 parse.cpp ./lib/lexer.cpp -Wall -Wextra -Werror
// ./a.out

Parser::Parser(std::string token){
    this->token = token;
}

Parser::~Parser(){
    
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
    std::string s;
    std::stringstream ss(token);
    std::vector<std::string> tokens;

    while (getline(ss, s, ' ')){
        tokens.push_back(s);
    }
    for (auto i: tokens){
            if (isOperator(std::string(i))){
                Node* oper = new Node(std::string(i));
                parseStack.push(oper);
            }
            else if (i == ")"){
                std::vector<Node*> tempVec;
                Node* temp = nullptr;
                while (parseStack.top() != nullptr && !parseStack.empty()){
                    temp = parseStack.top(); 
                    parseStack.pop();
                    if (isOperator(temp->data)){
                        if (temp->treeVec.empty()){
                            temp->treeVec = tempVec;
                            continue;
                        }
                    }
                    tempVec.push_back(temp);
                    }
                parseStack.pop();
                parseStack.push(temp);
                }

            else if (isNumber(i)){
               Node* digit = new Node(std::string(i));
                parseStack.push(digit);
            }
            else if (i== "("){
                parseStack.push(nullptr);
            }

    }
    return parseStack.top();
}


void printTreeInfix(Node* node) {
    if (!node) return;
    if (node->treeVec.size() > 0) std::cout << "(";
    for (int i = node->treeVec.size() - 1; i >= 0; --i) {
        printTreeInfix(node->treeVec[i]);
        if (i != 0) std::cout << " " << node->data << " ";
    }
    if (node->treeVec.size() > 0) std::cout << ")";
    if (node->treeVec.empty()) std::cout << node->data;
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
    // std::string str = "(* (+ 1 2) 3 (/ 4 5 (-6 7) ) )";
    // std::string str = "(+ 1 (* (* 2 3) 4))";
    // std::string str = "(+ 1 (* 2 (* 3 4)))";
    // std::string str = "(- 5 6 (+ 8 9.17 8)";
    // std::string str = "(+1(*234))";

    std::string line; // o store input
    int row = 0; 
    lexer Lexer;
    int new_line = 0; // 
    while(!std::cin.eof()) {
        //stores every instance of a new line
        new_line += 1;
        //goes through each line of input
        if(getline(std::cin, line)) { 
            row += 1;
            //makes tokens out of the line given
            //then puts them in tokenList
            Lexer.tokenize(row, line);
        }
    }
    if(new_line > row) {
        Lexer.tokenList.push_back(Tokens(new_line, 1, "END"));
    }
    else {
        Lexer.tokenList.push_back(Tokens(row, Lexer.tokenList.back().col+1, "END"));
    }
    

    std::string str;
    for (auto i : Lexer.tokenList){
        str += i.text + " ";
    }
    Parser parser(str);

    Node* root = parser.parse(str);
    printTreeInfix(root);
    std::cout<<std::endl<<parser.evaluate(root) << std::endl;


    
}