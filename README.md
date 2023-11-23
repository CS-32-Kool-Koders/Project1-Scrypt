Project1-Scrypt: Enhanced Scripting Language with Functions and Arrays
Introduction
Project1-Scrypt is an evolving C++ application that now embraces functions and resizable arrays, stepping into the realm of Turing completeness. This enhancement means the language can perform any computable calculation and offers a more human-friendly programming experience. The project demonstrates how basic calculators can evolve into sophisticated programming languages, handling multiple expressions, variable storage, control flow, functions, and arrays.

Installation and Setup
Follow these steps to install the updated version:

Ensure you have a C++ compiler and Make on your system.
Clone the repository to your local machine.
Navigate to the project directory and then to the src file.
Compile the project using the command:
bash
Copy code
g++ -std=c++17 -Wall -Wextra -Werror calc.cpp ./lib/infix_parser.cpp ./lib/lexer.cpp
Launch the program with ./a.out.
Usage
The program, executable from the command line, now includes functions and arrays alongside its previous features.

Functions
Functions are first-class citizens in this version. They can be defined, stored in variables, and passed as arguments. Functions capture variables from their defining scope, creating closures.

Example:

bash
Copy code
def add(x, y) {
  return x + y;
}

sum = add(5, 3);
print sum;  // Outputs 8
Arrays
Arrays are dynamic, allowing for resizing and nesting. They support array literals, lookups, and assignments.

Example:

bash
Copy code
array = [1, 2, 3];
array[1] = 5;
print array;  // Outputs [1, 5, 3]
Additional Utility Functions
len(array): Returns the length of the array.
pop(array): Removes and returns the last element of the array.
push(array, value): Adds a value to the end of the array.
Error Handling
The language robustly handles errors, such as type mismatches and out-of-bound array access.

Features
Function Support: Define and use functions, including nested and higher-order functions.
Array Handling: Create and manipulate dynamic arrays.
Enhanced Error Handling: Improved runtime error messages for better debugging.
Lexer and Parser Enhancements: Support for new tokens and syntax structures.
Development Stages
The development followed these stages:

Basic Calculator with lexer and parser.
Variable assignments and multi-expression handling.
Control flow with conditionals.
Completion with Functions and Arrays: Adding advanced features for comprehensive scripting capabilities.
Code Overview
The codebase is structured with various source files handling different aspects of the language:

infix_parser.cpp, calc.cpp, parse.cpp, lex.cpp: Core functionalities of parsing and calculating expressions.
blocks.cpp: Manages block structures in control flow.
scrypt.cpp: Central script processing unit.
format.cpp: Formats output for readability.
  

## Contact

For any inquiries or contributions, please contact the maintainers of this project.
Josh Herz - jherz@ucsb.edu
Skanda Vasishta - skandavasishta@ucsb.edu
Kanav Arora - kanav@ucsb.edu
Bharat Saiju - bsaiju@ucsb.edu
Vincent Mardirossian - vincentmardirossian@ucsb.edu
