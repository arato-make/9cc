#include "9cc.h"

Node *new_node(NodeKind kind, Node *lhs, Node *rhs) {
    Node *node = calloc(1, sizeof(Node));
    node->kind = kind;
    node->lhs = lhs;
    node->rhs = rhs;
    return node;
}

Node *new_node_num(int val) {
    Node *node = calloc(1,sizeof(Node));
    node->kind = ND_NUM;
    node->val = val;
    return node;
}

Node *new_node_ident(int offset) {
    Node *node = calloc(1,sizeof(Node));
    node->kind = ND_LVAR;
    node->offset = offset;
    return node;
}

Node *new_node_null() {
    Node *node = calloc(1,sizeof(Node));
    node->kind = ND_NULL;
    return node;
}

bool consume(char *op) {
    if (token->kind != TK_RESERVED || 
        strlen(op) != token->len ||
        memcmp(token->str, op, token->len))
        return false;
    token = token->next;
    return true;
}

bool check(char *op) {
    if (token->kind != TK_RESERVED || 
        strlen(op) != token->len ||
        memcmp(token->str, op, token->len))
        return false;
    return true;
}

void expect(char *op) {
    if(token->kind != TK_RESERVED || 
        strlen(op) != token->len ||
        memcmp(token->str, op, token->len))
        error_at(token->str, "'%c'ではありません", op);
    token = token->next;
}

int expect_number() {
    if (token->kind != TK_NUM)
        error_at(token->str, "数ではありません");
    int val = token->val;
    token = token->next;
    return val;
}

int get_offset() {
    LVar *lvar = find_lvar(token);
    int offset;
    if (lvar) {
        offset = lvar->offset;
    } else {
        lvar = calloc(1, sizeof(LVar));
        lvar->next = locals;
        lvar->name = token->str;
        lvar->len = token->len;
        lvar->offset = locals->offset + 8;
        offset = lvar->offset;
        locals = lvar;
    }
    token = token->next;
    return offset;
}

bool at_eof() {
    return token->kind == TK_EOF;
}

bool at_ident() {
    return token->kind == TK_IDENT;
}

bool at_return() {
    return token->kind == TK_RETURN;
}

LVar *find_lvar(Token *tok) {
    for (LVar *var = locals; var; var = var->next)
        if(var->len == tok->len && !memcmp(tok->str, var->name, var->len))
            return var;
    return NULL;
}

Node *program() {
    int i = 0;
    locals = calloc(1, sizeof(LVar));
    locals->offset = 0;
    while (!at_eof())
        code[i++] = stmt();
    code[i] = NULL;
}

Node *stmt() {
    Node *node;

    if (at_return()) {
        node = calloc(1,sizeof(Node));
        node->kind = ND_RETURN;
        token = token->next;
        node->lhs = expr();
    } else if (consume("if")) {
        if (consume("(")) {
            Node *condition_node = expr();
            if (!consume(")"))
                error_at(token->str, "')'ではないトークンです");
            
           
            if (consume("{"))
                node = block();
            else
                node = stmt();
            

            if (consume("else")) {
                if (consume("{"))
                    node = new_node(ND_ELSE, node, block());
                else
                    node = new_node(ND_ELSE, node, stmt());
            }

            node = new_node(ND_IF, condition_node, node);
            return node;
        } else {
            error_at(token->str, "'('ではないトークンです");
        }
    } else if (consume("while")) {
        if (consume("(")) {
            node = expr();
            if (!consume(")"))
                error_at(token->str, "')'ではないトークンです");

            if (consume("{"))
                node = new_node(ND_WHILE, node, block());
            else
                node = new_node(ND_WHILE, node, stmt());
            return node;
        } else {
            error_at(token->str, "'('ではないトークンです");
        }
    } else if (consume("for")) {
        if (consume("(")) {
            Node *initialize_node = expr();

            if (!consume(";")) 
                error_at(token->str, "';'ではないトークンです");

            Node *condition_node = expr();

            if (!consume(";")) 
                error_at(token->str, "';'ではないトークンです");

            Node *final_node = expr();

            if (!consume(")"))
                error_at(token->str, "')'ではないトークンです");

            if (consume("{"))
                node = new_node(ND_FOR, block(), final_node);
            else
                node = new_node(ND_FOR, stmt(), final_node);
            
            node = new_node(ND_FOR, condition_node, node);
            node = new_node(ND_FOR, initialize_node, node);

            return node;
        }
    } else {
        node = expr();
    }

    if (!consume(";")) {
        error_at(token->str, "';'ではないトークンです");
    }

    return node;
}

Node *block() {
    Node *node = stmt();
    if (check("}")){
        consume("}");
        return node;
    }
    return new_node(ND_BLOCK, node, block());
}


Node *expr() {
    return assign();
}

Node *assign() {
    Node *node = equality();
    if (consume("="))
        node = new_node(ND_ASSIGN, node, assign());
    return node;
}

Node *equality() {
    Node *node = relational();

    for (;;) {
        if (consume("=="))
            node = new_node(ND_EQ, node, relational());
        else if (consume("!="))
            node = new_node(ND_NEQ, node, relational());
        else
            return node;
    }
}

Node *relational() {
    Node *node = add();

    for (;;) {
        if (consume("<="))
            node = new_node(ND_LTE, node, add());
        else if (consume("<"))
            node = new_node(ND_LT, node, add());
        else if (consume(">="))
            node = new_node(ND_LTE, add(), node);
        else if (consume(">"))
            node = new_node(ND_LT, add(), node);
        else
            return node;
    }
}

Node *add() {
    Node *node = mul();

    for (;;) {
        if (consume("+"))
            node = new_node(ND_ADD, node, mul());
        else if (consume("-"))
            node = new_node(ND_SUB, node, mul());
        else
            return node;
    }
}

Node *mul() {
    Node *node = unary();

    for (;;) {
        if (consume("*"))
            node = new_node(ND_MUL, node, unary());
        else if (consume("/"))
            node = new_node(ND_DIV, node, unary());
        else
            return node;
    }
}

Node *primary() {
    if (consume("(")) {
        Node *node = expr();
        expect(")");
        return node;
    }

    if (at_ident()) {
        char *str = calloc(1, token->len);
        int len = token->len;
        strncpy(str, token->str, len);
        int offset = get_offset();
        if(consume("(")) {
            Node *node = calloc(1,sizeof(Node));
            node->kind = ND_FUNC;
            node->str = calloc(1, len);
            strcpy(node->str, str);

            if(!consume(")")){
                Node *arg = calloc(1,sizeof(Node));
                arg->kind = ND_ARG;
                arg->lhs = expr();
                node->rhs = arg;
                while (consume(",")) {
                    Node *arg_tmp = calloc(1,sizeof(Node));
                    arg_tmp->kind = ND_ARG;
                    arg_tmp->lhs = expr();
                    arg -> rhs = arg_tmp;
                    arg = arg_tmp;
                }
                arg -> rhs = new_node_null();
                consume(")");
            } else {
                node->rhs = new_node_null();
            }
            return node;
        }
        return new_node_ident(offset);
    }

    return new_node_num(expect_number());
}

Node *unary() {
    if (consume("+")) 
        return unary();
    if (consume("-"))
        return new_node(ND_SUB, new_node_num(0), unary());
    return primary();
}
