#include "9cc.h"

int arg_count;
char arg_reg[][4] = {
    "rdi",
    "rsi",
    "rdx",
    "rcx",
    "r8",
    "r9",
};

void gen_lval(Node *node) {
    if (node->kind != ND_LVAR)
        error("代入の左辺値が変数ではありません");

    printf("  mov rax, rbp\n");
    printf("  sub rax, %d\n", node->offset);
    printf("  push rax\n");
}

void gen(Node *node) {
    switch (node->kind)
    {
    case ND_NUM:
        printf("  push %d\n", node->val);
        return;
    case ND_LVAR:
        gen_lval(node);
        printf("  pop rax\n");
        printf("  mov rax, [rax]\n");
        printf("  push rax\n");
        return;
    case ND_ASSIGN:
        gen_lval(node->lhs);
        gen(node->rhs);
        
        printf("  pop rdi\n");
        printf("  pop rax\n");
        printf("  mov [rax], rdi\n");
        printf("  push rdi\n");
        return;
    case ND_RETURN:
        gen(node->lhs);
        printf("  pop rax\n");
        printf("  mov rsp, rbp\n");
        printf("  pop rbp\n");
        printf("  ret\n");
        return;
    case ND_IF:
        gen(node->lhs);
        printf("  pop rax\n");
        printf("  cmp rax, 0\n");
        if (node->rhs->kind == ND_ELSE) {
            gen(node->rhs);
        } else {
            printf("  je .Lend000\n");
            gen(node->rhs); 
        }
        printf(".Lend000:\n");
        return;
    case ND_ELSE:
        printf("  je .Lelse000\n");
        gen(node->lhs);
        printf("  jmp .Lend000\n");
        printf(".Lelse000:\n");
        gen(node->rhs);
        return;
    case ND_WHILE:
        printf(".Lbegin000:\n");
        gen(node->lhs);
        printf("  pop rax\n");
        printf("  cmp rax, 0\n");
        printf("  je .Lend000\n");
        gen(node->rhs);
        printf("  jmp .Lbegin000\n");
        printf(".Lend000:\n");
        return;
    case ND_FOR:
        gen(node->lhs);
        printf(".Lbegin000:\n");
        node = node->rhs;
        gen(node->lhs);
        printf("  pop rax\n");
        printf("  cmp rax, 0\n");
        printf("  je .Lend000\n");
        node = node->rhs;
        gen(node->rhs);
        gen(node->lhs);
        printf("  jmp .Lbegin000\n");
        printf(".Lend000:\n");
        return;
    case ND_BLOCK:
        gen(node->lhs);
        gen(node->rhs);
        return;
    case ND_FUNC:
        arg_count = 0;
        gen(node->rhs);
        printf("  call %s\n", node->str);
        printf("  push rax\n");
        return;
    case ND_ARG:
        gen(node->lhs);
        printf("  pop rax\n");
        printf("  mov %s, rax\n", arg_reg[arg_count]);
        arg_count++;
        gen(node->rhs);
        return;
    case ND_NULL:
        return;
    case ND_DEFF:
        printf("%s:\n", node->str);

        printf("  push rbp\n");
        printf("  mov rbp, rsp\n");
        printf("  sub rsp, 208\n");

        gen(node->lhs);

        printf("  pop rax\n");
    
        printf("  mov rsp, rbp\n");
        printf("  pop rbp\n");
        printf("  ret\n");
        return;
    }

    gen(node->lhs);
    gen(node->rhs);

    printf("  pop rdi\n");
    printf("  pop rax\n");

    switch (node->kind) {
    case ND_ADD:
        printf("  add rax, rdi\n");
        break;
    case ND_SUB:
        printf("  sub rax, rdi\n");
        break;
    case ND_MUL:
        printf("  imul rax, rdi\n");
        break;
    case ND_DIV:
        printf("  cqo\n");
        printf("  idiv rdi\n");
        break;
    case ND_EQ:
        printf("  cmp rax, rdi\n");
        printf("  sete al\n");
        printf("  movzb rax, al\n");
        break;
    case ND_NEQ:
        printf("  cmp rax, rdi\n");
        printf("  setne al\n");
        printf("  movzb rax, al\n");
        break;
    case ND_LT:
        printf("  cmp rax, rdi\n");
        printf("  setl al\n");
        printf("  movzb rax, al\n");
        break;
    case ND_LTE:
        printf("  cmp rax, rdi\n");
        printf("  setle al\n");
        printf("  movzb rax, al\n");
        break;
    }

    printf("  push rax\n");
}