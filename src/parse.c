#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#include <stdio.h>

#include "9hcc.h"

// read token and go next if next is expected token
// and return true nor false;
bool consume(char *op) {
  if (token->kind != TK_RESERVED ||
      token->len != strlen(op) ||
      memcmp(op, token->str, token->len)) {
    return false;
  }
  token = token->next;
  return true;
}

Token *consume_ident() {
  if (token->kind != TK_IDENT) {
    return false;
  }
  return token;
}

// read token if next token is expected,
// nor cause error
void expect(char *op) {
  if (token->kind != TK_RESERVED ||
      memcmp(token->str, op, token->len)) {
      error_at(token->str, "expected %.*s, but %s.", token->len, op, token->str);
      exit(1);
  }
  
  token = token->next;
}

// read token and go next if next is number
// nor cause error
int expect_number() {
  if (token->kind != TK_NUM) {
    error_at(token->str, "expected a number, but %s.", token->str);
  }
  int val = token->val;
  token = token->next;
  return val;
}

bool at_eof() { return token->kind == TK_EOF; }

// create new token and link cur
Token *new_token(TokenKind kind, Token *cur, char *str, int len) {
  Token *tok = calloc(1, sizeof(Token));
  tok->kind = kind; // set kind
  // set str
  tok->str = str;
  tok->len = len;
  tok->next = NULL;
  cur->next = tok;
  return cur->next;
}

// tokenize and return p
void tokenize(char *p) {
  // calloc head
  Token *head = calloc(1, sizeof(Token));
  token = head;
  Token *cur = token;

  while (*p) {
    // skip space
    if (isspace(*p)) {
      p++;
      continue;
    }
    // == != <= >=
    if (strncmp(p, "==", 2) == 0 ||
        strncmp(p, "!=", 2) == 0 ||
        strncmp(p, "<=", 2) == 0 ||
        strncmp(p, ">=", 2) == 0 ) {
      cur = new_token(TK_RESERVED, cur, p, 2);
      p+=2;
      continue;
    }

    // + - * / ( ) < > = ;
    if (*p == '+' || *p == '-' ||
        *p == '*' || *p == '/' ||
        *p == '(' || *p == ')' ||
        *p == '<' || *p == '>' ||
        *p == '=' || *p == ';') {
      cur = new_token(TK_RESERVED, cur, p, 1);
      p++;
      continue;
    }

    // indent
    if ('a' <= *p && *p <= 'z') {
      cur = new_token(TK_IDENT, cur, p, 1);
      p++;
      continue;
    }

    // number
    if (isdigit(*p)) {
      // 数値はstrに入れる必要がないのでlenに0をわたす
      cur = new_token(TK_NUM, cur, p, 0);
      cur->val = strtol(p, &p, 10);
      continue;
    }
    error_at(p, "cannot tokenize: '%c'", *p);
  }
  cur = new_token(TK_EOF, cur, p, 0);

  // 先頭が空っぽなので，一つズラす
  token = token->next;
}