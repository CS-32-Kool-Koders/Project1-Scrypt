#include "./lib/parser.h"
#include "./lib/lexer.h"
// g++ -std=c++17 parse.cpp ./lib/lexer.cpp -Wall -Wextra -Werror
// ./a.out

Parser::Parser(){
    // this->token = token;
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
// void errorHelper(std::vector<Tokens> tokenlist){

// }
bool Parser::isOperator(std::string c){
    return (c == "+")|| (c == "-") || (c == "*") || (c == "/") || (c == "=");
}


bool Parser::isNumber(std::string num) {
    int dec = 0;
    for (char c : num) {
        if (!isdigit(c) && c != '.') return false;
        if (c == '.') dec++;
    }
    return dec <= 1;
}

bool Parser::isIdentifier(std::string i){
    if (i == "END"){
        return false;
    }
    if (!isalpha(i[0]) && i[0] != '_'){
        return false;
    }
    for (auto c: i){
        if (!isalnum(c) && c != '_'){
            return false;
        }
    }
    return true;
}
// 
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
    int x = tokens.size();
    for (int i = 0; i < x; i++){
        if (isOperator(tokens.at(i))){
            Node* oper = nullptr;
        try {
            oper = new Node(tokens.at(i));
            parseStack.push(oper);

        } catch(...) {
            delete oper;  
            oper = nullptr;
            throw;  
        }
        }
        else if (tokens.at(i) == "("){
            // parenthesesCounter++;
            parseStack.push(nullptr);
        }
        else if (tokens.at(i) == ")"){
            std::vector<Node*> tempVec;
            Node* temp = nullptr;
            while (!parseStack.empty() && parseStack.top() != nullptr){
                temp = parseStack.top(); 
                parseStack.pop();
                if (isOperator(temp->data)){
                    if (temp->treeVec.empty()){
                        temp->treeVec = tempVec;
                        break;
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
        else if (isNumber(tokens.at(i))){
            Node* digit = nullptr;
            try{ 
                digit = new Node(tokens.at(i));
                parseStack.push(digit);
            } catch(...){
                delete digit;  
                digit  = nullptr;
                throw;
            }
        } 
        else if (isIdentifier(tokens.at(i))){
            // if (tokens.at(i-1) == "=" && tokens.at(i-2) == "(")
            Node* id = nullptr;
            id = new Node(tokens.at(i));
            parseStack.push(id);
        }
    }

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

// void Parser::makeMap(Node* root){
//     for (auto i : root->treeVec){
//         if (isIdentifier(i->data)){
//             variables[i->data] = 0;
//         }
//     }
// }

// double Parser::evaluate(Node* root, std::unordered_map<std::string, double>& variables) {
//     if (!root) {
//         std::cout << "Error: Root is a nullptr" << std::endl;
//         exit(1);
//     }
    
//     if (root->treeVec.empty()) {
//         if (root->data.size() == 1 && isdigit(root->data[0])) {
//             return std::stod(root->data);
//         } 
//         else if (isNumber(root->data)) {
//             return std::stod(root->data);
//         } 
//         else if (isIdentifier(root->data)) {
//             if(variables.find(root->data) == variables.end()) {
//                 std::cout << "Runtime error: unknown identifier " << root->data << std::endl;
//                 exit(3);
//              }
//             return variables[root->data]; 
            
//         }
//     }
//     double result;
//     if (root->data!="="){
//         result = evaluate(root->treeVec.back(), variables);
//     } else {
//         result = evaluate(root->treeVec.front(), variables);
//     }

//     for (int i = root->treeVec.size() - 2; i >= 0; --i) {
//         double operand = evaluate(root->treeVec[i], variables);
//         if (root->data == "+") {
//             result += operand;
//         } else if (root->data == "-") {
//             result -= operand;
//         } else if (root->data == "*") {
//             result *= operand;
//         } else if (root->data == "/") {
//             if (operand == 0) {
//                 std::cout << "Runtime error: dvision by zero." << std::endl;
//                 exit(3);
//             }
//             result /= operand;
//         } 
//         else if (root->data == "=") {
//             // int idx = root->treeVec.si
//             double res = evaluate(root->treeVec.front(), variables);
//             for (auto node : root->treeVec){
//                 //  std::cout<<"Node data"<<node->data<<std::endl;
//                 if (isIdentifier(node->data)){
//                     variables[node->data] = res;
                   
//                 }
//                 else{
//                     // break;
//                 }
//             }
//             result = res;
//         }
//     }

//     return result;
// }
double Parser::evaluate(Node* root, std::unordered_map<std::string, double>& variables, bool inAssignmentContext) {
    if (!root) {
        std::cout << "Error: Root is a nullptr" << std::endl;
        exit(1);
    }
    
    if (root->treeVec.empty()) {
        if (root->data.size() == 1 && isdigit(root->data[0])) {
            return std::stod(root->data);
        } 
        else if (isNumber(root->data)) {
            return std::stod(root->data);
        } 
        else if (isIdentifier(root->data)) {
            if(!inAssignmentContext && variables.find(root->data) == variables.end()) {
                std::cout << "Runtime error: unknown identifier " << root->data << std::endl;
                exit(3);
            }
            return variables[root->data];
        }
    }

    double result;
    if (root->data != "=") {
        result = evaluate(root->treeVec.back(), variables);
    } else {
        result = evaluate(root->treeVec.front(), variables, true);
    }

    for (int i = root->treeVec.size() - 2; i >= 0; --i) {
        double operand = evaluate(root->treeVec[i], variables, root->data == "=");
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
        else if (root->data == "=") {
            double res = evaluate(root->treeVec.back(), variables, true);
            for (auto node : root->treeVec){
                if (isIdentifier(node->data)) {
                    variables[node->data] = res;
                }
            }
            result = res;
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

    try {
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
    } catch(const std::runtime_error& e) {
        std::cout << e.what() << std::endl;
        return 1; // Exit with an error code, similar to the previous exit(1) calls
    }
    
    return 0; // Return 0 if there were no issues

    //start try
    // std::string line; 
    // int row = 0; 
    // lexer Lexer;
    // int new_line = 0; 
    // while(!std::cin.eof()) {
    //     new_line += 1;
    //     if(getline(std::cin, line)) { 
    //         row += 1;
    //         Lexer.tokenize(row, line);
    //     }
    // }
    // if(new_line > row) {
    //     Lexer.tokenList.push_back(Tokens(new_line, 1, "END"));
    // }
    // else {
    //     Lexer.tokenList.push_back(Tokens(row, Lexer.tokenList.back().col, "END"));
    // }
    //catch error here and exit 1
    
    // std::cout<<tokenList<<std::endl;
    int open = 0;
    int close = 0;
    std::string str;
    // std::vector<std::string> mult;
    // maybe add checks for multiple here, so they can be added to a vector
    for (auto i : Lexer.tokenList){
        // std::cout<<i.text<<std::endl;
        str += i.text + " ";
        // std::cout<<i.text<<std::endl;
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
    Parser parser; 

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
                (Lexer.tokenList[i+1].text != "(" && !parser.isNumber(Lexer.tokenList[i+1].text) && !parser.isOperator(Lexer.tokenList[i+1].text) && !parser.isIdentifier(Lexer.tokenList[i+1].text))) {
                reportUnexpectedToken(Lexer.tokenList[i]);
            }
        }
        if (Lexer.tokenList[i].text == "("){
            op++;
        }
        if (Lexer.tokenList[i].text == ")"){
            cl++;
        }

         if (parser.isOperator(Lexer.tokenList[i].text)) {
            numOperators++;
        }
        else if (parser.isNumber(Lexer.tokenList[i].text) || parser.isIdentifier(Lexer.tokenList[i].text)) { //add identifier shit here
            numNumbers++;
        }
        if (i == Lexer.tokenList.size()-1 && ((numNumbers == 0 && numOperators > 0) ||(numNumbers > 1 && numOperators == 0))) {
        std::cout << "Unexpected token at line " << Lexer.tokenList[i-1].line
                  << " column " << Lexer.tokenList[i-1].col << ": " <<Lexer.tokenList[i-1].text << std::endl;
        exit(2);
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

    //checks  problematic pattern (= 
    if (Lexer.tokenList[i].text == "(") {
        bool equalSignFound = false;
        bool numberFound = false;
        bool identifierFound = false;

        // Check for problematic pattern (= ...
        for (size_t j = i + 1; j < Lexer.tokenList.size() && Lexer.tokenList[j].text != ")"; j++) {
            if (Lexer.tokenList[j].text == "=") {
                equalSignFound = true;
            } else if (parser.isNumber(Lexer.tokenList[j].text)) {
                numberFound = true;
            } else if (parser.isIdentifier(Lexer.tokenList[j].text)) {
                identifierFound = true;
            }
        }

        if (equalSignFound && numberFound && !identifierFound) {
            std::cout << "Unexpected token at line " << Lexer.tokenList[i+2].line << " column " << Lexer.tokenList[i+2].col << ": " << Lexer.tokenList[i+2].text << std::endl;
            exit(2);
        }

        // Check for solo identifier in parentheses
        if (parser.isIdentifier(Lexer.tokenList[i+1].text) && Lexer.tokenList[i+2].text == ")") {
            std::cout << "Unexpected token at line " << Lexer.tokenList[i+1].line << " column " << Lexer.tokenList[i+1].col << ": " << Lexer.tokenList[i+1].text << std::endl;
            exit(2);
        }
    }
    }
    //Invalid assignment/assignee
    int o = 0;
    int c = 0;
    int id = 0;
    int n_ = 0;

    if (Lexer.tokenList.size() == 1 && parser.isNumber(Lexer.tokenList[0].text)) {
        std::cout << Lexer.tokenList[0].text << std::endl;  // print infix
        std::cout << Lexer.tokenList[0].text << std::endl;  // evaluate
        exit(0);
}

    for (size_t i = 0; i < Lexer.tokenList.size(); i ++){
        if (Lexer.tokenList[i].text == "("){
            o++;
        }
        if (Lexer.tokenList[i].text == ")"){
            c++;
        }
        if (parser.isIdentifier(Lexer.tokenList[i].text)){
            id++;
        }
        if (parser.isNumber(Lexer.tokenList[i].text)){
            n_++;
        }
        if (n_ == 0 && id !=0 && o == c){
            std::cout << "Unexpected token at line " << Lexer.tokenList[i].line << " column " << Lexer.tokenList[i].col << ": " << Lexer.tokenList[i].text << std::endl;
            exit(2);
        }
        if (parser.isNumber(Lexer.tokenList[i].text) && Lexer.tokenList.size() == 1) {
            std::cout << Lexer.tokenList[i].text << std::endl;  // print infix
            std::cout << Lexer.tokenList[i].text << std::endl;  // evaluate
            exit(0);  // exit the program since the processing for this input is complete
            }
            
        if (Lexer.tokenList[i].text == "="){
            for (size_t j = i+1; j < Lexer.tokenList.size(); j++){
                if (parser.isNumber(Lexer.tokenList[j].text)){
                    if (parser.isIdentifier(Lexer.tokenList[j+1].text) && Lexer.tokenList[j-1].text == "="){
                        std::cout << "Unexpected token at line " << Lexer.tokenList[j].line << " column " << Lexer.tokenList[j].col << ": " << Lexer.tokenList[j].text << std::endl;
                        exit(2);
                    } else if (parser.isIdentifier(Lexer.tokenList[j+1].text) && parser.isIdentifier(Lexer.tokenList[j-1].text)){ 
                        std::cout << "Unexpected token at line " << Lexer.tokenList[j+1].line << " column " << Lexer.tokenList[j+1].col << ": " << Lexer.tokenList[j+1].text << std::endl;
                        exit(2);
                    }
                }
            }
            if (!parser.isIdentifier(Lexer.tokenList[i+1].text)){
                std::cout << "Unexpected token at line " << Lexer.tokenList[i+1].line << " column " << Lexer.tokenList[i+1].col << ": " << Lexer.tokenList[i+1].text << std::endl;
                exit(2);
            }
            size_t idx = 0;
            bool open = false;
            size_t par = 0;
            for (size_t j = i+1; j <Lexer.tokenList.size(); j++){
                if (Lexer.tokenList[j].text == "("){
                    open = true;
                }
                if (Lexer.tokenList[j].text == ")"){
                    open = false;
                    idx = j;
                    par++;
                }
                if (parser.isNumber(Lexer.tokenList[j].text) && !open && j> idx && par == 1){
                    std::cout << "Unexpected token at line " << Lexer.tokenList[j].line << " column " << Lexer.tokenList[j].col << ": " << Lexer.tokenList[j].text << std::endl;
                    exit(2);
                }
            }
        }
    }

  
  
    std::vector<std::vector<Tokens>> separateExpressions;
    std::vector<Tokens> currentExpression;
    int depth = 0;
    // for (size_t i = 0; i < Lexer.tokenList.size(); i++){
    //         if (parser.isIdentifier(Lexer.tokenList[i].text)){
    //             parser.vars[Lexer.tokenList[i].text] = -69.6969;
    //         }
    //     }

    for (const auto& token : Lexer.tokenList) {
        if (token.text == "(") {
            depth++;
        } else if (token.text == ")") {
            depth--;
            if (depth < 0) {
                std::cerr << "Mismatched closing parenthesis." << std::endl;
                exit(2);
            }
        }

        if (depth == 0 && token.text == ")") {
            currentExpression.push_back(token);
            separateExpressions.push_back(currentExpression);
            currentExpression.clear();
        } else if (token.text != "END") {
            currentExpression.push_back(token);
        }
    }

    if (depth != 0) {
        std::cout << "Mismatched opening parenthesis." << std::endl;
        exit(2);
    }

    // Parser parser();
    for (const auto& expressionTokens : separateExpressions) {
        std::string expressionStr;
        for (const auto& token : expressionTokens) {
            expressionStr += token.text + " ";
        }

        // Parser parser(expressionStr);
        Node* root = parser.parse(expressionStr);

        if (root != nullptr) {
            parser.parseStack.pop();
        }

        printTreeInfix(root);
        // std::cout<<std::endl;
        // parser.makeMap(root);
        
        // double eval = parser.evaluate(root, parser.vars);
        // for (size_t i = 0; i < Lexer.tokenList.size(); i ++){
        //     if (parser.isIdentifier(Lexer.tokenList[i].text) && (parser.vars[Lexer.tokenList[i].text] == -69.6969)){
        //         std::cout<< "Runtime error: unknown identifier " << Lexer.tokenList[i].text << std::endl;
        //         exit(3);
        //     }
        // }
        std::cout << std::endl << parser.evaluate(root, parser.vars) << std::endl;
        delete root;
    }
    return 0;
}