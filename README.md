## 🔹 Mini Project 1 – TINY Language Scanner

### 📖 Overview
This project implements a **Lexical Analyzer (Scanner)** for the *TINY Programming Language*.  
It scans a TINY source code file, recognizes all valid tokens, and produces a structured list of **(tokenvalue, tokentype)** pairs.  
These tokens are then used as input for the **TINY Parser** in *Mini Project 2*.

---

### ⚙️ Features
✅ **Recognizes All TINY Tokens**  
Includes: `if`, `then`, `end`, `repeat`, `until`, `read`, `write`, identifiers, numbers, and all arithmetic/logical operators (`:=`, `<`, `=`, `+`, `-`, `*`, `/`, etc.)

✅ **Handles Whitespace & Comments**
- Block Comments `{ ... }`  
- Line Comments `// ...`

✅ **Detects Unknown Symbols** → Marked as `Unknown`  
✅ **Appends an End-of-File Marker** → `End_of_file`

---

### 🧰 Technical Details
| Property | Description |
|-----------|--------------|
| **Language** | C++ |
| **Compiler** | GNU GCC (`g++`) |
| **Execution Environment** | Windows (PowerShell / Git Bash / Command Prompt) |
| **Input File** | `input.txt` (TINY source code) |
| **Output File** | `output.txt` (list of tokens) |

---

### 💻 How to Compile and Run

#### 🧩 Step 1 — **Compile the Scanner**
Open your terminal inside the project folder and run:
```bash
g++ -o scanner.exe scanner.cpp
