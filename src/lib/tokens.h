#include <iostream>

struct Tokens
{
  int line;
  int col;
  std::string text;

  Tokens(int line, int col, std::string text)
  {
    this->line = line;
    this->col = col;
    this->text = text;
  }
};
