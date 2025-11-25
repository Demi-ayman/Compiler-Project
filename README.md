# 🔹 1- TINY Language Scanner

This project implements a Lexical Analyzer (Scanner) for the *TINY Programming Language*.  
It scans a TINY source code file, recognizes all valid tokens, and produces a structured list of (tokenvalue, tokentype) pairs.  
These tokens are then used as input for the TINY Parser.

---

## ⚙️ Features
✅ Recognizes All TINY Tokens  
Includes: if, then, end, repeat, until, read, write, identifiers, numbers, and all arithmetic/logical operators (:=, <, =, +, -, *, /, etc.)

✅ Handles Whitespace & Comments
- Block Comments { ... }  
- Line Comments // ...

✅ Detects Unknown Symbols → Marked as Unknown  
✅ Appends an End-of-File Marker → End_of_file

---

## 🧰 Technical Details
| Property | Description |
|-----------|--------------|
| Language | C++ |
| Compiler | GNU GCC (g++) |
| Execution Environment | Windows (PowerShell / Git Bash / Command Prompt) |
| Input File | input.txt (TINY source code) |
| Output File | output.txt (list of tokens) |

---

## 💻 How to Compile and Run

Open your terminal inside the project folder and run:
g++ -o scanner.exe scanner.cpp

---
---

# 🔹 2- TINY Language Parser  

This project implements a Recursive Descent Parser for the TINY programming language using an EBNF grammar.  
The parser reads a list of tokens, checks whether the input program matches the grammar, reports syntax errors, and generates a Syntax Tree.  
A GUI version is also included to allow users to parse tokens visually and display the syntax tree.

---

## 📘 Features

### ✔️ 1. Full EBNF Grammar Support  
The parser implements all rules of the TINY language:

---

### ✔️ 2. Recursive Descent Parser Architecture  
Each grammar rule is implemented as a separate function, including:

- parseProgram()  
- parseStmtSequence()  
- parseStatement()  
- parseIfStmt()  
- parseRepeatStmt()  
- parseAssignStmt()  
- parseExp()  
- parseSimpleExp()  
- parseTerm()  
- parseFactor()  

---

### ✔️ 3. Syntax Tree Generation  
After successful parsing:

- a full Abstract Syntax Tree (AST) is created  
- each node stores rule name or operator  
- children represent sub-expressions or nested statements  
- the tree is printed in a hierarchical tree-like format  

---

### ✔️ 4. Error Handling System  
The parser provides detailed syntax error messages for:

- Missing tokens (then, end, until)  
- Missing expressions after :=  
- Unknown tokens  
- Invalid operator positions  
- Unbalanced parentheses  
- Extra or missing semicolons  
- Unexpected token sequences  

All errors show:

- token value  
- token type  
- line number  
- what was expected

---

### ✔️ 5. GUI Application  
The GUI provides:

- Textbox to input tokens manually  
- Load file button to import tokens  
- Parse button  
- Display area showing the Syntax Tree  
- Error message panel  

---

## 📝 Input Token File Format

Each line must follow:
value,TYPE

---

## ▶️ How to Run the Parser

### Compile:
g++ parser.cpp -o parser.exe

### Run:
parser.exe tokens.txt

---

## ✔️ Output Results

### If input program is valid:
Accepted. Syntax tree: ...

### If input program is invalid:
Parser error: Syntax error at token () line ()
Expected factor: ()
