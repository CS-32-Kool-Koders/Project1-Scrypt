// ast shit here
#include "lib/blocks.h"
// Blocks::Blocks(ExpressionParser *left){
//     // this->left = left;
// }
Blocks::~Blocks()
{
    // Delete the condition if it was allocated dynamically
    if (condition != nullptr)
    {
        delete condition;
    }

    // Delete the thenBlock if it was allocated dynamically
    // if (thenBlock != nullptr)
    // {
    //     delete thenBlock;
    // }

    // Delete the elseBlock if it was allocated dynamically
    // if (elseBlock != nullptr)
    // {
    //     delete elseBlock;
    // }

    // Delete all dynamically allocated Blocks in blocklist
    for (Blocks *block : blocklist)
    {
        delete block;
    }
}