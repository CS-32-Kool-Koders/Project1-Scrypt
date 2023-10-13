#include <iostream>
#include <vector>
#include <string>
#include "tokens.h"
class lexer {
    public:
        std::vector<Tokens> tokenList;
        lexer();
        void tokenize(int row, std::string line);
    private:
        int checkIsDigit(std::string line, int row, int col);   

};
