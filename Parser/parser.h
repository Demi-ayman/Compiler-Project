#ifndef PARSER_H
#define PARSER_H

#include <string>
#include <vector>
#include <stdexcept>

// ---------- Token types ----------
enum class TokenType {
    SEMICOLON,
    IF,
    THEN,
    ELSE,
    END,
    REPEAT,
    UNTIL,
    IDENTIFIER,
    ASSIGN,   // :=
    READ,
    WRITE,
    LESSTHAN, // <
    EQUAL,    // =
    PLUS,
    MINUS,
    MULT,
    DIV,
    OPENBRACKET,  // (
    CLOSEDBRACKET,// )
    NUMBER,
    END_OF_FILE,
    UNKNOWN
};

struct Token{
    std::string value;
    TokenType type;
    int lineNo;
};

struct Node{
    std::string type;
    std::string value;
    std::vector<Node*> children;
    Node(const std::string &t, const std::string &v = "") : type(t ),value(v){}
};

extern std::vector<Token> tokens;
extern int currentIndex;

void loadTokensFromFile(const std::string &path);
Node* parseProgram();
void freeTree(Node* node);

#endif // PARSER_H
