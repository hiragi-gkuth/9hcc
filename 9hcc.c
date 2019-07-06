#include <ctype.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef enum {
	      TK_RESERVED,
	      TK_NUM,
	      TK_EOF,
} TokenKind;

typedef struct Token Token;

struct Token {
  TokenKind kind;
  Token *next;
  int val;
  char *str;
};

// token variable
Token *token;

/// Error handling
// get argument as well as printf
void error(char *fmt, ...) {
  va_list ap;
  va_start(ap, fmt);
  vfprintf(stderr, fmt, ap);
  fprintf(stderr, "\n");
  exit(1);
}

// read token and go next if next is expected token
// and return true nor false;
bool consume(char op) {
  if (token->kind != TK_RESERVED || token->str[0] != op) {
    return false;
  }
  token = token->next;
  return true;
}

// read token if next token is expected,
// nor cause error
void expect(char op) {
  if (token->kind != TK_RESERVED || token->str[0] != op) {
    error("not a '%c'.", op);
  }
  token = token->next;
}

// read token and go next if next is number
// nor cause error
int expect_number() {
  if (token->kind != TK_NUM) {
    error("not a number");
  }
  int val = token->val;
  token = token->next;
  return val;
}

bool at_eof() {
  return token->kind == TK_EOF;
}

// create new token and link cur
Token *new_token(TokenKind kind, Token *cur, char *str) {
  Token *tok = calloc(1, sizeof(Token));
  tok->kind = kind;
  tok->str = str;
  cur->next = tok;
  return tok;
}

// tokenize and return p
Token *tokenize(char *p) {
  Token head;
  head.next = NULL;
  Token *cur = &head;

  while (*p) {
    // skip space
    if (isspace(*p)) {
      p++;
      continue;
    }
    // + or -
    if (*p == '+' || *p == '-') {
      cur = new_token(TK_RESERVED, cur, p++);
      continue;
    }

    // number
    if (isdigit(*p)) {
      cur = new_token(TK_NUM, cur, p);
      cur->val = strtol(p, &p, 10);
      continue;
    }
    error("cannot tokenize");
  }

  new_token(TK_EOF, cur, p);
  return head.next;
}


int main(int argc, char **argv) {
  if (argc != 2) {
    fprintf(stderr, "Invalid argument count");
    return 1;
  }

  token = tokenize(argv[1]);

  // asm header
  printf(".intel_syntax noprefix\n");
  printf(".global main\n");
  printf("main:\n");

  // check if first is num
  printf("  mov rax, %d\n", expect_number());

  // consume tokens
  // output asm
  while (!at_eof()) {
    if (consume('+')) {
      printf("  add rax, %d\n", expect_number());
      continue;
    }

    expect('-');
    printf("  sub rax, %d\n", expect_number());
  }

  printf("  ret\n");
  return 0;
}
