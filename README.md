# Project1-Scrypt

A simple project made with friends - a lightweight scripting language.

## Checkpoint 4

The program is a C++ application designed to perform calculations and provide a comprehensive understanding of compiler design principles, particularly in parsing and executing a scripting language. It utilizes a parser and lexer for mathematical expressions. It provides functionality to parse and evaluate expressions written in both S-expressions and infix notation, and supports variable assignments and basic arithmetic operations.

Seeing the forest through the trees...

Taking a step back, the larger purpose of writing this program is to demonstrate the implementation of a programming language. Breaking this down into more digestable checkpoints such as a calculator and parser helps us undestand the fundamentals of a functional programming language. For example, this project helps us understand how programming languages can store more than one expression at a time, being able to save results by storing them in a variable.

## Installation

To install and set up the project, follow these steps:

1. Ensure you have a C++ compiler and make installed on your system.

2. Clone the repository to your local machine.

3. Navigate to the project directory. Then, navigate to the src file.

4. Compile the project using the command `g++ -std=c++17 -Wall -Wextra -Werror calc.cpp ./lib/infix_parser.cpp ./lib/lexer.cpp`

5. Launch program with `./a.out`

```bash

git clone <repository-url>

cd  project-1-scrypt/src

g++ -std=c++17 -Wall -Wextra -Werror calc.cpp \
    ./lib/infix_parser.cpp \
    ./lib/lexer.cpp`

./a.out
```

## Usage

The program can be run from the command line, and it provides two main functionalities:

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

### 4. Array Evaluation

Example input:

```bash
(array = [true, 2, ((1 + 1) + 1), 4, [5]]);
print array[2];
```

Example output:

```bash
3
```

## Features

### Calculator Component

- **S-Expression Parsing**: Parses and evaluates mathematical expressions written in S-expressions.

- **Infix Notation Parsing**: Parses and evaluates mathematical expressions written in infix notation.

- **Variable Assignment**: Supports assignment of values to variables and subsequent use of these variables in expressions.

- **Basic Arithmetic Operations**: Supports addition, subtraction, multiplication, and division operations.

- **Lexer**: Tokenizes input strings into meaningful components for parsing.

### Persistence

- Allows saving the state of the program such as variables and their values.

### Conditionals

- Conditional Statements: Introduces if, else, and elif statements for decision-making processes.

- Boolean Expressions: Supports evaluating expressions that result in boolean values for use in conditional statements.

- Complex Conditional Logic: Enables the creation of more complex and nuanced decision-making structures in the program.

### Completion (Part 4)

- Functions and Arrays: Adds support for defining and using functions, as well as creating and manipulating arrays.

- Turing Completeness: Implements features that make the language Turing complete, meaning it can calculate anything computable.

- Scope and Closures: Introduces the concept of local and global scope, with functions being closures capturing their enclosing scope.

- Error Handling: Enhances robustness through runtime error detection and handling for various operations, especially related to functions and arrays.



#### Array Class

This source file contains constructors and functions for the purpose of implementing a basic array structure.
Member Functions:

- len(): Returns the size of the array.
- operator[]: Provides array element access via index.
- push(int value): Adds a new element to the end of the array.
- pop(): Removes and returns the last element of the array.

#### Main Function

This source file contains constructors and functions for the purpose of implementing a basic array structure.
Member Functions:

- Array Creation: An array is instantiated with an initializer list.
- Element Access: Demonstrates accessing elements using the array index.
- Printing the Array: Iterates through the array to print its contents.
- Modifying Elements: Swaps two elements and shows the modified array.
- Push and Pop Operations: Illustrates pushing a new element to the array and popping the last element.

### `scrypt.cpp`

This source file serves as the backbone of the script interpreter, coordinating between lexical analysis, parsing, and execution. It nables the interpretation and execution of a custom scripting language, handling conditional statements, loops, and expressions, working with lexer.h for tokenization and blocks.cpp for representing parsed statements.

#### Main Function (int main()):

This is the entry point of the script interpreter.
It reads input lines, tokenizes them using lexer.h, and organizes tokens into statements.
The main function then constructs an Abstract Syntax Tree (AST) by parsing these statements into Blocks.

#### Parsing Statements (parseStatements function):

This function recursively constructs the AST from the tokens.
It handles different types of statements like if, while, else, and print, as well as expressions.
Each type of statement is parsed into a Blocks object with appropriate properties set (like condition and blocklist for conditional blocks).

### `infix_parser.cpp`

This source file contains the implementation of the parser for infix notation. It converts a sequence of tokens into an abstract syntax tree (AST) representing the mathematical expression, which can then be evaluated.

### `calc.cpp`

This source file contains the main function for the infix notation calculator. It takes user input, tokenizes it, parses it into an AST, and then evaluates the expression.

### `parse.cpp`

This source file contains the main function for the S-expression calculator. Similar to `calc.cpp`, it takes user input, tokenizes it, parses it into an AST, and evaluates the expression.

### `lex.cpp`

This source file contains the main function for the lexer. It takes a string as input, tokenizes it, and prints out the tokens.

### Other Source Files

- `lexer.cpp`: Implementation of the lexer.

- `parser.h`, `tokens.h`, `infix_parser.h`, `lexer.h`, `arr.h`, `blocks.h`: Header files declaring the functions and classes used in the implementation.

## Contact

For any inquiries or contributions, please contact the maintainers of this project.
Josh Herz - jherz@ucsb.edu
Skanda Vasishta - skandavasishta@ucsb.edu
Kanav Arora - kanav@ucsb.edu
Bharat Saiju - bsaiju@ucsb.edu
Vincent Mardirossian - vincentmardirossian@ucsb.edu
