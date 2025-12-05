#include "parser.h"


// command line input: parser.exe "input examples\valid\ex1.txt"

#include <bits/stdc++.h>
using namespace std;

// ---------- Token types ----------
// enum class TokenType {
//     SEMICOLON,
//     IF,
//     THEN,
//     ELSE,
//     END,
//     REPEAT,
//     UNTIL,
//     IDENTIFIER,
//     ASSIGN,   // :=
//     READ,
//     WRITE,
//     LESSTHAN, // <
//     EQUAL,    // =
//     PLUS,
//     MINUS,
//     MULT,
//     DIV,
//     OPENBRACKET,  // (
//     CLOSEDBRACKET,// )
//     NUMBER,
//     END_OF_FILE,
//     UNKNOWN
// };

string tokenTypeToString(TokenType t) {
    switch(t){
    case TokenType::SEMICOLON: return "SEMICOLON";
    case TokenType::IF: return "IF";
    case TokenType::THEN: return "THEN";
    case TokenType::ELSE: return "ELSE";
    case TokenType::END: return "END";
    case TokenType::REPEAT: return "REPEAT";
    case TokenType::UNTIL: return "UNTIL";
    case TokenType::IDENTIFIER: return "IDENTIFIER";
    case TokenType::ASSIGN: return "ASSIGN";
    case TokenType::READ: return "READ";
    case TokenType::WRITE: return "WRITE";
    case TokenType::LESSTHAN: return "LESSTHAN";
    case TokenType::EQUAL: return "EQUAL";
    case TokenType::PLUS: return "PLUS";
    case TokenType::MINUS: return "MINUS";
    case TokenType::MULT: return "MULT";
    case TokenType::DIV: return "DIV";
    case TokenType::OPENBRACKET: return "OPENBRACKET";
    case TokenType::CLOSEDBRACKET: return "CLOSEDBRACKET";
    case TokenType::NUMBER: return "NUMBER";
    case TokenType::END_OF_FILE: return "EOF";
    default: return "UNKNOWN";
    }
}

// ---------- Token struct ----------
// struct Token {
//     string value;
//     TokenType type;
//     int lineNo;
// };

// Simple map from text token type string to TokenType enum
TokenType stringToTokenType(const string &s) {
    string t = s;
    // normalize uppercase
    for (auto &c : t) c = toupper((unsigned char)c);
    if (t == "SEMICOLON") return TokenType::SEMICOLON;
    if (t == "IF") return TokenType::IF;
    if (t == "THEN") return TokenType::THEN;
    if (t == "ELSE") return TokenType::ELSE;
    if (t == "END") return TokenType::END;
    if (t == "REPEAT") return TokenType::REPEAT;
    if (t == "UNTIL") return TokenType::UNTIL;
    if (t == "IDENTIFIER") return TokenType::IDENTIFIER;
    if (t == "ASSIGN") return TokenType::ASSIGN;
    if (t == "READ") return TokenType::READ;
    if (t == "WRITE") return TokenType::WRITE;
    if (t == "LESSTHAN") return TokenType::LESSTHAN;
    if (t == "EQUAL") return TokenType::EQUAL;
    if (t == "PLUS") return TokenType::PLUS;
    if (t == "MINUS") return TokenType::MINUS;
    if (t == "MULT") return TokenType::MULT;
    if (t == "DIV") return TokenType::DIV;
    if (t == "OPENBRACKET") return TokenType::OPENBRACKET;
    if (t == "CLOSEDBRACKET") return TokenType::CLOSEDBRACKET;
    if (t == "NUMBER") return TokenType::NUMBER;
    return TokenType::UNKNOWN;
}

// ---------- AST Node ----------
// struct Node {
//     string type;               // e.g., "assign", "if", "plus", "number"
//     string value;              // for identifiers/numbers
//     vector<Node*> children;
//     Node(const string &t, const string &v = "") : type(t), value(v) {}
// };

// Utility to print AST (indentation)
void printTree(Node* node, int indent = 0) {
    if (!node) return;
    cout << string(indent, ' ');
    cout << node->type;
    if (!node->value.empty()) cout << " (" << node->value << ")";
    cout << "\n";
    for (auto c : node->children) printTree(c, indent + 4);
}

// Free nodes
void freeTree(Node* node) {
    if (!node) return;
    for (auto c : node->children) freeTree(c);
    delete node;
}

// ---------- Parser state ----------
vector<Token> tokens;
int currentIndex = 0;

Token currentToken() {
    if (currentIndex < 0) currentIndex = 0;
    if (currentIndex >= (int)tokens.size()) {
        return Token{"", TokenType::END_OF_FILE, (int)tokens.size()};
    }
    return tokens[currentIndex];
}

void advance() {
    if (currentIndex < (int)tokens.size()) currentIndex++;
}

void error(const string &msg) {
    Token t = currentToken();
    string s = "Syntax error";
    if (t.type != TokenType::END_OF_FILE) {
        s += " at token '" + t.value + "' (type=" + tokenTypeToString(t.type) + ")";
        s += ", line " + to_string(t.lineNo);
    } else {
        s += " at EOF";
    }
    s += " -> " + msg;
    throw runtime_error(s);
}

void match(TokenType expected) {
    Token t = currentToken();
    if (t.type == expected) {
        advance();
    } else {
        error("expected token type " + tokenTypeToString(expected));
    }
}

// ---------- Parser (Recursive Descent) ----------
// We implement EBNF rules.
// Each parseX returns Node* representing that construct.
// On error, throw runtime_error with message.

// Forward declarations
Node* parseProgram();
Node* parseStmtSequence();
Node* parseStatement();
Node* parseIfStmt();
Node* parseRepeatStmt();
Node* parseAssignStmt();
Node* parseReadStmt();
Node* parseWriteStmt();
Node* parseExp();
Node* parseSimpleExp();
Node* parseTerm();
Node* parseFactor();

// program → stmt-sequence
Node* parseProgram() {
    Node* root = new Node("program");
    Node* seq = parseStmtSequence();
    root->children.push_back(seq);
    // expect EOF afterwards
    if (currentToken().type != TokenType::END_OF_FILE) {
        // allow trailing semicolons possibly but generally throw if more tokens remain
        // If remaining tokens are only SEMICOLON, consume them.
        while (currentToken().type == TokenType::SEMICOLON) advance();
        if (currentToken().type != TokenType::END_OF_FILE) {
            error("extra tokens after end of program");
        }
    }
    return root;
}

// stmt-sequence → statement { ; statement }
Node* parseStmtSequence() {
    Node* seq = new Node("stmt-sequence");
    Node* first = parseStatement();
    seq->children.push_back(first);
    while (currentToken().type == TokenType::SEMICOLON) {
        match(TokenType::SEMICOLON);
        // allow trailing semicolon before END etc.
        // if next is END, ELSE, UNTIL, EOF -> break (empty statement not allowed)
        TokenType t = currentToken().type;
        if (t == TokenType::END || t == TokenType::ELSE || t == TokenType::UNTIL || t == TokenType::END_OF_FILE) {
            // allow semicolon before END (common in tiny examples), break
            break;
        }
        Node* st = parseStatement();
        seq->children.push_back(st);
    }
    return seq;
}

// statement→ if-stmt | repeat-stmt | assign-stmt | read-stmt | write-stmt
Node* parseStatement() {
    Token t = currentToken();
    switch (t.type) {
    case TokenType::IF:
        return parseIfStmt();
    case TokenType::REPEAT:
        return parseRepeatStmt();
    case TokenType::IDENTIFIER:
        return parseAssignStmt();
    case TokenType::READ:
        return parseReadStmt();
    case TokenType::WRITE:
        return parseWriteStmt();
    default:
        error("expected start of a statement (IF/REPEAT/IDENTIFIER/READ/WRITE)");
    }
    return nullptr;
}

// if -stmt → IF exp THEN stmt-sequence [ELSE stmt-sequence] END
Node* parseIfStmt() {
    match(TokenType::IF);
    Node* node = new Node("if");
    Node* cond = parseExp();
    node->children.push_back(new Node("condition"));
    node->children.back()->children.push_back(cond);

    match(TokenType::THEN);
    Node* thenSeq = parseStmtSequence();
    node->children.push_back(new Node("then"));
    node->children.back()->children.push_back(thenSeq);

    if (currentToken().type == TokenType::ELSE) {
        match(TokenType::ELSE);
        Node* elseSeq = parseStmtSequence();
        node->children.push_back(new Node("else"));
        node->children.back()->children.push_back(elseSeq);
    }

    match(TokenType::END);
    return node;
}

// repeat -stmt → REPEAT stmt-sequence UNTIL exp
Node* parseRepeatStmt() {
    match(TokenType::REPEAT);
    Node* node = new Node("repeat");
    Node* seq = parseStmtSequence();
    node->children.push_back(seq);
    match(TokenType::UNTIL);
    Node* cond = parseExp();
    node->children.push_back(cond);
    return node;
}

// assign -stmt → IDENTIFIER ASSIGN exp
Node* parseAssignStmt() {
    Token id = currentToken();
    if (id.type != TokenType::IDENTIFIER) error("expected identifier at start of assignment");
    string varname = id.value;
    match(TokenType::IDENTIFIER);
    match(TokenType::ASSIGN);
    Node* node = new Node("assign");
    node->value = varname;
    Node* expr = parseExp();
    node->children.push_back(expr);
    return node;
}

// read -stmt → READ IDENTIFIER
Node* parseReadStmt() {
    match(TokenType::READ);
    Token id = currentToken();
    if (id.type != TokenType::IDENTIFIER) error("expected identifier after READ");
    Node* node = new Node("read", id.value);
    match(TokenType::IDENTIFIER);
    return node;
}

// write -stmt → WRITE exp
Node* parseWriteStmt() {
    match(TokenType::WRITE);
    Node* node = new Node("write");
    Node* expr = parseExp();
    node->children.push_back(expr);
    return node;
}

// exp → simple-exp [ comparison-op simple-exp ]
Node* parseExp() {
    Node* left = parseSimpleExp();
    Token t = currentToken();
    if (t.type == TokenType::LESSTHAN || t.type == TokenType::EQUAL) {
        Node* comp = new Node("comparison", tokenTypeToString(t.type));
        if (t.type == TokenType::LESSTHAN) comp->value = "<";
        else comp->value = "=";
        advance();
        Node* right = parseSimpleExp();
        comp->children.push_back(left);
        comp->children.push_back(right);
        return comp;
    } else {
        return left;
    }
}

// simple-exp → term { addop term }
Node* parseSimpleExp() {
    Node* node = parseTerm();
    while (currentToken().type == TokenType::PLUS || currentToken().type == TokenType::MINUS) {
        Token op = currentToken();
        advance();
        Node* right = parseTerm();
        Node* parent = new Node(op.type == TokenType::PLUS ? "add" : "sub");
        parent->children.push_back(node);
        parent->children.push_back(right);
        node = parent;
    }
    return node;
}

// term → factor { mulop factor }
Node* parseTerm() {
    Node* node = parseFactor();
    while (currentToken().type == TokenType::MULT || currentToken().type == TokenType::DIV) {
        Token op = currentToken();
        advance();
        Node* right = parseFactor();
        Node* parent = new Node(op.type == TokenType::MULT ? "mul" : "div");
        parent->children.push_back(node);
        parent->children.push_back(right);
        node = parent;
    }
    return node;
}

// factor → ( exp ) | number | identifier
Node* parseFactor() {
    Token t = currentToken();
    if (t.type == TokenType::OPENBRACKET) {
        match(TokenType::OPENBRACKET);
        Node* e = parseExp();
        match(TokenType::CLOSEDBRACKET);
        return e;
    } else if (t.type == TokenType::NUMBER) {
        Node* n = new Node("number", t.value);
        match(TokenType::NUMBER);
        return n;
    } else if (t.type == TokenType::IDENTIFIER) {
        Node* id = new Node("identifier", t.value);
        match(TokenType::IDENTIFIER);
        return id;
    } else {
        error("expected factor: (exp) or number or identifier");
    }
    return nullptr;
}

// ---------- Utility: read tokens from a file ----------
// Expected input file format: each line "value,TYPE"
// - value may contain characters (e.g. :=, ;, x, 12, ( )
// - TYPE is one of the token type names used in PDF (IDENTIFIER, NUMBER, ASSIGN, SEMICOLON, IF, THEN, ...)
// Lines starting with # or empty lines are ignored.
void loadTokensFromFile(const string &path) {
    tokens.clear();
    ifstream in(path);
    if (!in.is_open()) {
        throw runtime_error("Could not open tokens file: " + path);
    }
    string line;
    int lineno = 0;
    while (getline(in, line)) {
        lineno++;
        // trim
        auto l = line;
        while (!l.empty() && isspace((unsigned char)l.back())) l.pop_back();
        size_t pos = l.find(',');
        if (pos == string::npos) {
            // ignore empty/comment
            string t = l;
            // trim leading
            size_t i = 0; while (i < t.size() && isspace((unsigned char)t[i])) i++;
            if (i >= t.size()) continue;
            if (t[i] == '#') continue;
            // otherwise error: must be "value,TYPE"
            throw runtime_error("Invalid token format at line " + to_string(lineno) + ": '" + line + "'");
        }
        string val = l.substr(0, pos);
        string typ = l.substr(pos + 1);
        // trim spaces
        auto trim = [](string &s) {
            size_t a = 0; while (a < s.size() && isspace((unsigned char)s[a])) a++;
            size_t b = s.size(); while (b> a && isspace((unsigned char)s[b-1])) b--;
            s = s.substr(a, b-a);
        };
        trim(val); trim(typ);
        if (val.empty() && typ.empty()) continue;
        Token tk;
        tk.value = val;
        tk.type = stringToTokenType(typ);
        tk.lineNo = lineno;
        if (tk.type == TokenType::UNKNOWN) {
            // in case user used actual symbol like "<" as token type instead of LESSTHAN
            string tmp = typ;
            for (auto &c : tmp) c = toupper((unsigned char)c);
            if (tmp == "<") tk.type = TokenType::LESSTHAN;
            else if (tmp == "=") tk.type = TokenType::EQUAL;
            else if (tmp == ";") tk.type = TokenType::SEMICOLON;
            else if (tmp == ":=") tk.type = TokenType::ASSIGN;
            else if (tmp == "(") tk.type = TokenType::OPENBRACKET;
            else if (tmp == ")") tk.type = TokenType::CLOSEDBRACKET;
            else if (tmp == "+") tk.type = TokenType::PLUS;
            else if (tmp == "-") tk.type = TokenType::MINUS;
            else if (tmp == "*") tk.type = TokenType::MULT;
            else if (tmp == "/") tk.type = TokenType::DIV;
        }
        tokens.push_back(tk);
    }
    // Append EOF token
    tokens.push_back(Token{"", TokenType::END_OF_FILE, (int)tokens.size()+1});
}

// // ---------- Main ----------
// int main(int argc, char** argv) {
//     try {
//         if (argc < 2) {
//             cerr << "Usage: " << argv[0] << " tokens.txt\n";
//             cerr << "Format per line: value,TYPE  (e.g. x,IDENTIFIER or 12,NUMBER or :=,ASSIGN or ;,SEMICOLON)\n";
//             return 1;
//         }
//         loadTokensFromFile(argv[1]);
//         currentIndex = 0;
//         Node* root = parseProgram();

//         cout << "Accepted. Syntax tree:\n";
//         printTree(root);

//         freeTree(root);
//         return 0;
//     } catch (const exception &ex) {
//         cerr << "Parser error: " << ex.what() << "\n";
//         return 2;
//     }
// }

