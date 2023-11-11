#include <iostream>
// #include "lib/infix_parser.h"
#include "lib/lexer.h"
#include "lib/blocks.h"
#include <string>
#include <sstream>
#include <algorithm>

bool isStatement(const std::string& tokenText){
    return tokenText == "if" || tokenText == "while" || tokenText == "else";
}
void checkTokenString(const std::string &tokenString);
Blocks* parseBlock(std::vector<std::vector<Tokens>>& lines, size_t& lineIndex, size_t& tokenIndex);

// Blocks* parseStatements(std::vector<Tokens>& tokens, const size_t& i=0);
Blocks* parseStatements(std::vector<std::vector<Tokens>>& lines, size_t& lineIndex, size_t& tokenIndex);

bool checkoperator(std::string op);
BooleanWrapper evaluateExpression(ExpressionParser* parser);
void evaluateBlock(Blocks* block);



int main() {
    std::string line; 
    int row = 0; 
    lexer Lexer;
    int new_line = 0; 
    try{
        while(!std::cin.eof()) {
            new_line += 1;
            if(getline(std::cin, line)) { 
                row += 1;
                Lexer.tokenize(row, line);
            }
        }
    } 
    catch(const std::runtime_error& e){
        std::cout << e.what() << std::endl;
        exit(1);
    }
    if(new_line > row) {
        Lexer.tokenList.push_back(Tokens(new_line, 1, "END"));
    }
    else {
        Lexer.tokenList.push_back(Tokens(row, Lexer.tokenList.back().col+1, "END"));
    }
    

    //go through token list
    //add to some string, if end, then 
     // std::cout << "tokenlist size = " << Lexer.tokenList.size() << std::endl;
     // std::cout << "tokenlist size = " << Lexer.tokenList.size() << std::endl;
    std::vector<std::vector<Tokens>> tokensByLine;
    std::vector<Tokens> temp;
    temp.push_back(Lexer.tokenList.at(0));
    for (size_t i = 1; i < Lexer.tokenList.size(); i++)
    {
        // std::cout << "i = " << i << std::endl;
        if (!checkoperator(Lexer.tokenList.at(i - 1).text) && !checkoperator(Lexer.tokenList.at(i).text))
        {
            // std::cout << "boop" << std::endl;
            tokensByLine.push_back(temp);
            temp.clear();
            temp.push_back(Lexer.tokenList.at(i));
        }
        // std::cout << "can't get past me" << std::endl;
        if (i == Lexer.tokenList.size() - 1)
        {
            // std::cout << "owowow" << std::endl;
            if (!checkoperator(Lexer.tokenList.at(i).text))
            {
                // std::cout << "end of the line for you there, buddy" << std::endl;
                temp.push_back(Lexer.tokenList.at(i));
                tokensByLine.push_back(temp);
                temp.clear();
            }
            tokensByLine.push_back(temp);
            temp.clear();
            i++;
        }
        else
        {
            if (checkoperator(Lexer.tokenList.at(i).text) && !checkoperator(Lexer.tokenList.at(i + 1).text))
            {
                // std::cout << "beep" << std::endl;
                temp.push_back(Lexer.tokenList.at(i));
                temp.push_back(Lexer.tokenList.at(i + 1));
                i++;
            }
        }
    }
    for (std::vector<Tokens> vec : tokensByLine)
    {
        std::cout << "new vector" << std::endl;
        for (Tokens token : vec)
        {
            std::cout << token.line << " " << token.col << " " << token.text << std::endl;
        }
        std::cout << "end vector" << std::endl;
    } 

    // std::vector<Blocks*> bList;
    // for (std::vector<Tokens> vec : tokensByLine){
    //     Blocks* tempBlock(nullptr);
    //     // tempBlock = parseStatements(vec);
    //     bList.push_back(tempBlock);
    // }
    // for (auto &Block: bList){
    //     for(auto Token: Block->tokenlist){
    //         std::cout << Token.text << std::endl;
    //     }
    // }

    std::vector<Blocks*> astNodes;
    size_t lineIndex = 0, tokenIndex = 0;
    while (lineIndex < tokensByLine.size()) {
        Blocks* astNode = nullptr;
        if (!tokensByLine[lineIndex].empty() && tokensByLine[lineIndex][0].text == "{") {
            // If the line starts with a block opening
            astNode = parseBlock(tokensByLine, lineIndex, tokenIndex);
        } else {
            // For regular statements or expressions
            astNode = parseStatements(tokensByLine, lineIndex, tokenIndex);
            lineIndex++;  // Move to the next line
        }
        if (astNode) {
            astNodes.push_back(astNode);
        }
    }
    
    // for (Blocks* rootBlock : astNodes) {
    //     std::cout<<"This is a block"<<std::endl;
    //     std::cout<<rootBlock->type<<std::endl;
    // }
    for (Blocks* rootBlock : astNodes) {
        evaluateBlock(rootBlock);
    }

    // Clean-up (if necessary)
    // for (Blocks* node : astNodes) {
    //     delete node;  // Assuming dynamic allocation in parseStatements and parseBlock
    // }
    return 0;
}

bool checkoperator(std::string op){
    std::vector<std::string> ops = {
    "+", "-", "*", "/", "=", "END", "%",
    "==", ">", ">=", "<", "<=", "|", "^", "&", "!=",
    "+", "-", "*", "/", "=", "END", "%", "==", ">", 
    ">=", "<", "<=", "|", "^", "&", "!=",
    "+", "-", "*", "/", "=", "%", "==", ">", ">=",
     "<", "<=", "|", "^", "&", "!="};
    if (std::find(ops.begin(), ops.end(), op) == ops.end()) {
        return false;
    }
    return true;
}


Blocks* parseStatements(std::vector<std::vector<Tokens>>& lines, size_t& lineIndex, size_t& tokenIndex) {
    if (lineIndex >= lines.size()) return nullptr;

    std::vector<Tokens>& tokens = lines[lineIndex];
    if (tokenIndex >= tokens.size()) {
        lineIndex++;
        tokenIndex = 0;
        if (lineIndex >= lines.size()) return nullptr;
        tokens = lines[lineIndex];
    }

    Blocks* block = new Blocks();
    if (tokens[tokenIndex].text == "if" || tokens[tokenIndex].text == "while") {
        block->type = tokens[tokenIndex].text;
        tokenIndex++;
        // Extract the condition tokens until '{'
        std::vector<Tokens> conditionTokens;
        // Assuming '{' comes immediately after the condition
        while (tokenIndex < tokens.size() && tokens[tokenIndex].text != "{") {
            conditionTokens.push_back(tokens[tokenIndex]);
            tokenIndex++;
        }
        block->condition = new ExpressionParser(conditionTokens);
        if (tokenIndex < tokens.size() && tokens[tokenIndex].text == "{") {
            lineIndex++;
            tokenIndex = 0;
        }
        
        // Parse the statements inside the block
        block->thenBlock = parseBlock(lines, lineIndex, tokenIndex);

        // Handle 'else' for 'if' statements
        if (block->type == "if" && lineIndex < lines.size()) {
            std::vector<Tokens>& nextTokens = lines[lineIndex];
            if (nextTokens[tokenIndex].text == "else") {
                tokenIndex++;
                if (nextTokens[tokenIndex].text == "{") {
                    lineIndex++;
                    tokenIndex = 0;
                    block->elseBlock = parseBlock(lines, lineIndex, tokenIndex);
                } else if (nextTokens[tokenIndex].text == "if") {
                    block->elseBlock = parseStatements(lines, lineIndex, tokenIndex);
                }
            }
        }
    } else if (tokens[tokenIndex].text == "print") {
        block->type = "print";
        tokenIndex++; 
        block->condition = new ExpressionParser(tokens); 
    } else {
        block->type = "expression";
        block->condition = new ExpressionParser(tokens);
    }

    lineIndex++; 
    tokenIndex = 0;
    return block;
}

Blocks* parseBlock(std::vector<std::vector<Tokens>>& lines, size_t& lineIndex, size_t& tokenIndex) {
    Blocks* block = new Blocks();
    block->type = "block";

    while (lineIndex < lines.size() && lines[lineIndex][0].text != "}") {
        Blocks* statement = parseStatements(lines, lineIndex, tokenIndex);
        if (statement) {
            block->blocklist.push_back(statement);
        }
    }

    if (lineIndex < lines.size() && lines[lineIndex][0].text == "}") {
        lineIndex++;  // Move to the next line after the block
        tokenIndex = 0;
    }

    return block;
}

void evaluateBlock(Blocks* block) {
    if (!block) return;

    BooleanWrapper resultVar;
    if (block->type == "if" || block->type == "while") {
        // Assuming block->condition is an ExpressionParser instance
        ExpressionNode* root = block->condition->parseExpression();
        if (root != nullptr) {
            root->getVariablesNames();
            root->computeInfix();
            resultVar = root->computeResult();
            delete root;
        }

        if (block->type == "if") {
            if (resultVar.getBvalue()) {
                evaluateBlock(block->thenBlock);
            } else if (block->elseBlock) {
                evaluateBlock(block->elseBlock);
            }
        } else if (block->type == "while" && resultVar.getBvalue()) {
            do {
                evaluateBlock(block->thenBlock);
                // Re-evaluate condition for while loop
                root = block->condition->parseExpression();
                root->getVariablesNames();
                root->computeInfix();
                resultVar = root->computeResult();
                delete root;
            } while (resultVar.getBvalue());
        }
    } else if (block->type == "print") {
        // Evaluate print expression
        ExpressionNode* root = block->condition->parseExpression();
        if (root != nullptr) {
            root->getVariablesNames();
            root->computeInfix();
            resultVar = root->computeResult();
            root->printResult(resultVar); // Print result
            delete root;
        }
    } else if (block->type == "expression") {
        // Evaluate standalone expression
        ExpressionNode* root = block->condition->parseExpression();
        if (root != nullptr) {
            root->getVariablesNames();
            root->computeInfix();
            resultVar = root->computeResult();
            delete root;
        }
    }

    // Handle other block types if any
}
