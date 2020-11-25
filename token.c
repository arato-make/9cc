#include "9cc.h"

Token *new_token(TokenKind kind, Token *cur, char *str, int len) {
    Token *tok = calloc(1, sizeof(Token));
    tok->kind = kind;
    tok->str = str;
    user_input = str;
    tok->len = len;
    cur->next = tok;
    return tok;
}

int is_alnum(char c) {
    return ('a' <= c && c <= 'z' ||
        ('A' <= c && c <= 'Z') ||
        ('0' <= c && c <= '9') ||
        (c == '_'));
}

Token *tokenize() {
    char *p = user_input;
    Token head;
    head.next = NULL;
    Token *cur = &head;

    while (*p) {
        if (isspace(*p)) {
            p++;
            continue;
        }

        if (memcmp(p, "==", 2) == 0 ||
            memcmp(p, "!=", 2) == 0|| 
            memcmp(p, "<=", 2) ==0 || 
            memcmp(p, ">=", 2) == 0 ) {
            cur = new_token(TK_RESERVED, cur, p, 2);
            p += 2;
            continue;
        }

        if (strchr("+-*/()<>=;", *p)) {
            cur = new_token(TK_RESERVED, cur, p++, 1);
            continue;
        }

        if (isdigit(*p)) {
            cur = new_token(TK_NUM, cur, p, 1);
            cur->val = strtol(p, &p, 10);
            continue;
        }

        if (memcmp(p, "return", 6) == 0 && !is_alnum(p[6])) {
            cur = new_token(TK_RETURN, cur, p, 6);
            p += 6;
            continue;
        }

        if ('a' <= *p && *p <= 'z') {
            int i = 0;
            while (is_alnum(*p)) {
                i++;
                p++;
            }
            p -= i;

            cur = new_token(TK_IDENT, cur, p, i);

            p += i;
            continue;
        }

        error_at(p, "トークナイズできません");
    }

    new_token(TK_EOF, cur, p, 0);
    return head.next;
}
