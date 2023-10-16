#include <vector>
#include <string>
#include "tokens.h"
class lexer {
    public:
        //storing tokens
        std::vector<Tokens> tokenList; 
        // function that takes line of input
        // and then makes tokens from that line
        // and then inputs that into tokenList
        int tokenize(int row, std::string line);
        std::vector<Tokens> printTokenList();
    private:
        //handles numbers and decimals
        int checkIsDigit(std::string line, int row, int col);   

};
