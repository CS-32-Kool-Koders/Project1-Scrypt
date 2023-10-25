#include <vector>
#include <string>
#include "tokens.h"
class lexer {
    public:
        //storing tokens
        // function that takes line of input
        // and then makes tokens from that line
        // and then inputs that into tokenList
        std::vector<Tokens> tokenList;
        int tokenize(int row, std::string line);
    private:
        //handles numbers and decimals
        int checkIsDigit(std::string line, int row, int col);  
        int checkIsIdentifier(std::string line, int row, int col) ;
};
