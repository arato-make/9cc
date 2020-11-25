#include <ctype.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef enum {
    TK_RESERVED,
    TK_IDENT,
    TK_NUM,
    TK_EOF,
} TokenKind;

typedef enum {
    ND_ADD,
    ND_SUB,
    ND_MUL,
    ND_DIV,
    ND_NUM,
    ND_EQ,
    ND_NEQ,
    ND_LT,
    ND_LTE,
    ND_ASSIGN,
    ND_LVAR,
} NodeKind;

typedef struct Token Token;
struct Token {
    TokenKind kind;
    Token *next;
    int val;
    char *str;
    int len;
};

typedef struct Node Node;
struct Node {
    NodeKind kind;
    Node *lhs;
    Node *rhs;
    int val;
    int offset;
};


Token * token;
char *user_input;
Node *code[100];

bool consume();
void expect();
int expect_number();
int get_offset();
bool at_eof();
bool at_ident();
Token *tokenize();

Node *program();
Node *stmt();
Node *expr();
Node *assign();
Node *equality();
Node *relational();
Node *add();
Node *mul();
Node *primary();
Node *unary();

void gen();

void error(char *fmt, ...);
void error_at(char *loc, char *fmt, ...);