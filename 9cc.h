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
    TK_RETURN,
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
    ND_RETURN,
    ND_IF,
    ND_ELSE,
    ND_WHILE,
    ND_FOR,
    ND_BLOCK,
    ND_FUNC,
    ND_ARG,
    ND_NULL,
    ND_DEFF,
} NodeKind;

typedef enum {
    FUNC_CALL,
    FUNC_DEFF,
} FuncKind;

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
    char *str;
    int offset;
};

typedef struct LVar LVar;
struct LVar {
    LVar *next;
    char *name;
    int len;
    int offset;
};

LVar *locals;
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

LVar *find_lvar(Token *tok);
Node *program();
Node *stmt();
Node *block();
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