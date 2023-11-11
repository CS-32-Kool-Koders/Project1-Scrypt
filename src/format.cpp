#include <iostream>
#include <iomanip>
#include "lib/infix_parser.h"
#include "lib/lexer.h"
#include <string>
#include <sstream>
#include <vector>
#include "lib/blocks.h"

// Assuming 'Tokens', 'Blocks', and other necessary classes are defined in the included headers

// Forward declarations
void printAST(const Blocks* block, int indent = 0);
Blocks* buildAST(const std::vector<Tokens>& /* tokens */);
void printExpression(const ExpressionParser* /* expr */);

int main() {
    std::string line; 
    int row = 0; 
    lexer Lexer;

    while(getline(std::cin, line)) { 
        row += 1;
        Lexer.tokenize(row, line);
    }

    // Build the AST from the tokens
    // Blocks* root = buildAST(Lexer.tokenList);

    // // Print the AST with proper formatting
    // printAST(root);

    // Cleanup
    // delete root;

    return 0;
}

// Implement this function to build the AST from the tokens
Blocks* buildAST(const std::vector<Tokens>& /* tokens */) {
    // Add your logic to build the AST from the tokens
    return new Blocks();  // Placeholder return
}

// Recursive function to print the AST with indentation
void printAST(const Blocks* block, int indent) {
    if (!block) return;

    std::string indentation(indent, ' ');

    if (block->type == "if" || block->type == "while") {
        std::cout << indentation << block->type << " (";
        printExpression(block->condition);
        std::cout << ") {" << std::endl;

        if (block->thenBlock) {
            printAST(block->thenBlock, indent + 4);
        }
        if (block->elseBlock) {
            std::cout << indentation << "} else {" << std::endl;
            printAST(block->elseBlock, indent + 4);
        }
        std::cout << indentation << "}" << std::endl;
    } else {
        // Handling other types of blocks if necessary
        for (const auto& childBlock : block->blocklist) {
            printAST(childBlock, indent);
        }
    }
}

// Implement this function based on your ExpressionParser and how it represents expressions
void printExpression(const ExpressionParser* /* expr */) {
    // Your logic to traverse and print the expression
}