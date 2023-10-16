#include <iostream>

struct Tokens {
  int line; //storing line of token
  int col; //storing column of token
  std::string text; //storing what the token is ex. ")", "2", "+", "-"
  
  Tokens(int line, int col, std::string text) {
    this->line = line;
    this->col = col;
    this->text = text;
  }
};
