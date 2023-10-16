#include "./lib/parser.h"


Parser::Parser(std::string token){
    this->token = token;
}

Parser::~Parser(){
    
}

Node::Node(char data){
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

bool Parser::isOperator(char c){
    return (c == '+')|| (c == '-') || (c == '*') || (c == '/');
}

bool Parser::isNumber(std::string num){
    if (num.length() > 1){
        int dec = 0;
        for (char c: num){
            if (!(isdigit(c) || c == '.')){
                return false;
            if (c == '.'){
                dec ++;
            }
            }
        }
    if (dec > 1){
        return false;
    }
    }
    return true;
}

Node* Parser::parse(std::string token){
    
    for (char i: token){
        if (isOperator(i)){
            Node* oper = new Node(i);
            parseStack.push(oper);
        }
        else if (i == ')'){
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

        else if (isdigit(i)){
            Node* digit = new Node(i);
            parseStack.push(digit);
        }
        else if (i == '('){
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
        if (isdigit(root->data)) {
            return std::stod(std::string(1, root->data));
        }
    }

    double result = evaluate(root->treeVec.back());  // Start from the last element
    for (int i = root->treeVec.size() - 2; i >= 0; --i) { // Traverse in reverse
        double operand = evaluate(root->treeVec[i]);
        if (root->data == '+') {
            result += operand;
        } else if (root->data == '-') {
            result -= operand;
        } else if (root->data == '*') {
            result *= operand;
        } else if (root->data == '/') {
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
    std::string str = "(* (+ 1 2) 3 (/ 4 5 (-6 7) ) )";
    // std::string str = "(+ 1 (* (* 2 3) 4))";
    // std::string str = "(+ 1 (* 2 (* 3 4)))";
    // std::string str = "(- 5 6 (+ 8 9.17 8)";
    // std::string str = "(+1(*234))";
    Parser parser(str);

    Node* root = parser.parse(str);
    printTreeInfix(root);
    std::cout<<std::endl<<parser.evaluate(root);


    
}
