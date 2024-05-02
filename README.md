# Ankr :anchor:

## Overview

Ankr is a custom programming language interpreter designed to execute programs written in the Ankr language. The interpreter processes Ankr source code through a sequence of steps to tokenize, parse, and execute the code. Below is a breakdown of the interpreter's workflow:

## Workflow

### 1. Input Reading

The interpreter starts by reading an Ankr source file, which typically has the extension `.ankr`. This file contains the code that will be executed.

### 2. Tokenization

The raw text from the input file is processed into a series of tokens. This involves scanning the text and recognizing different elements such as keywords, operators, literals, and identifiers, converting them into a structured format that can be used in later stages.

### 3. Parsing

Tokens are fed into the parser, which constructs an Abstract Syntax Tree (AST) based on the grammatical rules of the Ankr language. The parser verifies the syntactic structure of the token sequence and organizes them into a hierarchical tree that represents the program's logical structure.

### 4. Abstract Syntax Tree (AST)

The AST is a tree representation of the syntactic structure of the Ankr program. Each node in the tree corresponds to a language construct, such as expressions, control flow statements, function declarations, etc.

### 5. AST Traversal and Interpretation

The interpreter traverses the AST from the root, interpreting each node according to its type. This process involves executing operations like arithmetic computations, variable allocations, and function calls as defined by the nodes of the AST.

### 6. Scope and State Management

The interpreter manages the program's state—including variable values and function definitions—using a stack-like structure to handle scope. This allows the interpreter to manage local and global variables efficiently and to support nested scopes essential for function calls and control blocks.

### 7. Execution

The final phase involves executing the program by traversing the AST. Starting at the root node of the AST, each node is visited based on it's type. The state of the program as well as the scope is updated after executing expressions, functions, etc.

## Features

- **Dynamic Typing**: Ankr supports dynamic typing and offers basic types such as integers, floats, strings, and booleans.
- **Control Structures**: Includes if-else, for, and while loops.
- **Functions**: Support for user-defined functions with local scoping.
- **Built-in Functions**: Includes input/output functions, random, and basic math operations.

## Getting Started

### Prerequisites

- C++17 compatible compiler (GCC, Clang, MSVC)

### Building the Project

Clone the repository and navigate to the project directory:

```bash
git clone https://github.com/yourusername/ankr.git
cd ankr
make
```

### Running Ankr

After building, you can run the Ankr interpreter with:

```
./ankr path/to/your/script.ankr
```

## Example Code

```
function fibonacci(n) {
  var a = 0;
  var b = 1;
  var f = 1;
  for (var i = 2; i <= n; i++) {
    f = a + b;
    a = b;
    b = f;
  }
  return f;
}

// Calculate and print first 10 fibonacci numbers
for (var i = 1; i < 10; i++) {
  output("Fibonacci " + i + ": " + fibonacci(i));
}
```
