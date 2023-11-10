//ast shit here
#include "lib/blocks.h"
Blocks::Blocks(ExpressionParser *left){
    this->left = left;
}
Blocks::~Blocks(){
    delete this->left;
}
void Blocks::execute(std::vector<Tokens> tokens){
    
}