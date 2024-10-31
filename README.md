# Scrypt
A lightweight scripting language and compiler built from scratch with C++

## Introduction
The program is a C++ application designed to perform calculations and utilize a parser and lexer for mathematical expressions. It provides functionality to parse and evaluate expressions written in both S-expressions and infix notation, and supports variable assignments and basic arithmetic operations. It also showcases the implementation of various programming language features starting from basic arithmetic operations to advanced functionalities like control flow, variables, and functions.

Taking a step back, the larger purpose of writing this program is to demonstrate the implementation of a programming language. Breaking this down into more digestable checkpoints such as a calculator and parser helps us undestand the fundamentals of a functional programming language. For example, checkpoint 2 helps us understand how programming languages can store more than one expression at a time, being able to save results by storing themin variable.

## Installation
To install and set up the project, follow these steps:

1. Ensure you have a C++ compiler and make installed on your system.

2. Clone the repository to your local machine.

3. Navigate to the project directory. Then, navigate to the src file. 

4. ~~Compile the project using the provided Makefile.~~ (WIP)

5. Compile the project using the command `g++ -std=c++17 -Wall -Wextra -Werror calc.cpp ./lib/infix_parser.cpp ./lib/lexer.cpp`

6. Launch program with `./a.out`

```bash

git clone <repository-url>

cd  project-1-scrypt/src

g++ -std=c++17 -Wall -Wextra -Werror calc.cpp \
    ./lib/infix_parser.cpp \
    ./lib/lexer.cpp`

./a.out
```


 

## Usage

The program can be run from the command line, and it provides the following functionalities

  

### 1. S-Expression Calculator (`parse.cpp`)

This functionality takes S-expressions as input, evaluates them, and returns the result.

  

```bash

./parse

// Example input
(+3(*45)) 

// Interpretation by Calculator
       +
      / \
     3   *
        / \
       4   5

-> 23
```
#### Tree Representation:

This tree represents the structure of the S-expression and the order of operations that takes place when the tree is evaluated. 

#### Transliterated Representation:
1.  Evaluate (∗ 4 5), resulting in 20.
2.  Add 3 to the result from step 1: 3+20=**23**


  

### 2. Infix Notation Calculator (`calc.cpp`)

This functionality takes expressions in infix notation as input, evaluates them, and returns the result.

  

```bash

./calc

// Example Input
(1+2)  

// Example Output
1 + 2
3
```

  

### 3. Lexer (`lex.cpp`)

This functionality takes a string as input and parses it into different tokens.

  

```bash

./lex

(+ 1  2) // Example input

'(', '+', '1', '2', ')' //Interpretation by Lexer

```

### 4. Using Variables


Input:
```bash
x = 5
y = x + 3
y
```
Output:
```bash
x = 5
y = x + 3
8
```

In this example, the program shows variable assignment and usage. It stores the value 5 in x, then computes y as x + 3, and finally outputs the value of y.

### 5. Conditional Statements
Input:
```bash
if (x > 3) { x + 2 } else { x - 2 }
```
Output:
```bash
7
```
This input demonstrates a simple conditional statement. Since x is 5 (from the previous example), the condition x > 3 is true, and the program outputs the result of x + 2.

## Features

-  **S-Expression Parsing**: Parses and evaluates mathematical expressions written in S-expressions.

-  **Infix Notation Parsing**: Parses and evaluates mathematical expressions written in infix notation.

-  **Variable Assignment**: Supports assignment of values to variables and subsequent use of these variables in expressions.

-  **Basic Arithmetic Operations**: Supports addition, subtraction, multiplication, and division operations.

-  **Lexer**: Tokenizes input strings into meaningful components for parsing.

-  **Conditional Statements**: Includes conditional logic with `if`, `else`, and related control flow constructs. Provides the ability to execute different code paths based on Boolean conditions.

### Loops and Iteration (if applicable)

### Development Stages
The development of Project1-Scrypt was divided into four stages, each adding layers of complexity and features:

1.  Calculator Implementation: Building a basic calculator with lexer and parser.
2.  Persistence Features: Introducing variables and multiple expression handling.
3.  Conditionals: Adding Boolean values and control flow elements like if and while.
4.  Completion with Functions and Arrays: Implementing functions and arrays to enhance the language's capabilities.
  

## Code Overview

### `infix_parser.cpp`

This source file contains the implementation of the parser for infix notation. It converts a sequence of tokens into an abstract syntax tree (AST) representing the mathematical expression, which can then be evaluated.

  

### `calc.cpp`

This source file contains the main function for the infix notation calculator. It takes user input, tokenizes it, parses it into an AST, and then evaluates the expression.

  

### `parse.cpp`

This source file contains the main function for the S-expression calculator. Similar to `calc.cpp`, it takes user input, tokenizes it, parses it into an AST, and evaluates the expression.

  

### `lex.cpp`

This source file contains the main function for the lexer. It takes a string as input, tokenizes it, and prints out the tokens.




### `blocks.cpp`
This manages block structures within the code, essential for control flow.




### `scrypt.cpp`
This is the central script processing unit, combining lexer and parser components.




### `format.cpp`
This handles formatting of outputs.
  

### Other Source Files

-  `lexer.cpp`: Implementation of the lexer.

-  `parser.h`, `tokens.h`, `infix_parser.h`, `lexer.h`, `blocks.h`: Header files declaring the functions and classes used in the implementation.

  

## Contact

For any inquiries or contributions, please contact the maintainers of this project.
Josh Herz - jherz@ucsb.edu
Skanda Vasishta - skandavasishta@ucsb.edu
Kanav Arora - kanav@ucsb.edu
Bharat Saiju - bsaiju@ucsb.edu
Vincent Mardirossian - vincentmardirossian@ucsb.edu
