#include <iostream>
#include <vector>
#include <string>
#include "infix_parser.h"

struct Blocks
{
    std::string type;
    ExpressionParser *condition;
    // Blocks* thenBlock;
    // Blocks* elseBlock;
    std::vector<Blocks *> blocklist;

    Blocks() : condition(nullptr) {}
    ~Blocks()
    {
        delete condition;
    }
    void execute(std::vector<Tokens> tokens);
    // void recursiveDebuggingPrint()
    // {
    //     if (!this->blocklist.empty())
    //     {
    //         for (Blocks *block : this->blocklist)
    //         {
    //             std::cout << "Type: " << block->type << std::endl;
    //             block->recursiveDebuggingPrint();
    //         }
    //     }
    //     else
    //     {
    //         for (Tokens& token : this->condition->getTokens())
    //         {
    //             std::cout << token.text << " ";
    //         }
    //         std::cout << std::endl;
    //     }
    // }
    // void printInfo() const {
    // std::cout << "Type: " << type << std::endl;

    // if (condition) {
    //     std::cout << "Condition Tokens: ";
    //     for (const Tokens& token : condition->getTokens()) {
    //         std::cout << token.text << " ";
    //     }
    //     std::cout << std::endl;
    // }

    // if (!blocklist.empty()) {
    //     std::cout << "Blocklist:" << std::endl;
    //     for (const Blocks* block : blocklist) {
    //         block->printInfo();  // Recursive call for nested blocks
    //     }
    //}
    //}
};
