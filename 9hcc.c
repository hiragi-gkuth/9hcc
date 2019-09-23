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
  int len;
};

typedef enum {
  ND_EQ,  // ==
  ND_NEQ, // !=
  ND_LT,  // <
  ND_LE,  // <=
  ND_ADD, // +
  ND_SUB, // -
  ND_MUL, // *
  ND_DIV, // /
  ND_NUM, // [1-9]*
} NodeKind;

typedef struct Node Node;

struct Node {
  NodeKind kind;
  Node *lhs;
  Node *rhs;
  int val;
};

// prototypes
Node *expr();
Node *mul();
Node *unary();
Node *primary();

// token variable
Token *token;

// user input
char *user_input;

// notify error pos
void error_at(char *loc, char *fmt, ...) {
  va_list ap;
  va_start(ap, fmt);

  int pos = loc - user_input;
  fprintf(stderr, "%s\n", user_input);
  fprintf(stderr, "%*s", pos, "");
  fprintf(stderr, "^ ");
  vfprintf(stderr, fmt, ap);
  fprintf(stderr, "\n");
  exit(1);
}

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
bool consume(char *op) {
  if (token->kind != TK_RESERVED ||
      token->len != strlen(op) ||
      memcmp(op, token->str, token->len)) {
    return false;
  }
  token = token->next;
  return true;
}

// read token if next token is expected,
// nor cause error
void expect(char *op) {
  if (token->kind != TK_RESERVED ||
      memcmp(token->str, op, token->len)) {
    error_at(token->str, "not a '%c'.", op);
  }
  token = token->next;
}

// read token and go next if next is number
// nor cause error
int expect_number() {
  if (token->kind != TK_NUM) {
    error_at(token->str, "not a number");
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
  tok->str = malloc(sizeof(char) * (len + 1));
  strncpy(tok->str, str, len); tok->str[len] = '\0';
  tok->len = len;
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
    // == != <= >=
    if (strncmp(p, "==", 2) == 0 ||
        strncmp(p, "!=", 2) == 0 ||
        strncmp(p, "<=", 2) == 0 ||
        strncmp(p, ">=", 2) == 0 ) {
      cur = new_token(TK_RESERVED, cur, p, 2);
      p+=2;
      continue;
    }

    // + - * / ( ) < >
    if (*p == '+' || *p == '-' ||
        *p == '*' || *p == '/' ||
        *p == '(' || *p == ')' ||
        *p == '<' || *p == '>') {
      cur = new_token(TK_RESERVED, cur, p++, 1);
      continue;
    }

    // number
    if (isdigit(*p)) {
      // 数値はstrに入れる必要がないのでlenに0をわたす
      cur = new_token(TK_NUM, cur, p, 0);
      cur->val = strtol(p, &p, 10);
      continue;
    }
    error_at(p, "cannot tokenize");
  }

  new_token(TK_EOF, cur, p, 0);
  return head.next;
}

Node *new_node(NodeKind kind, Node *lhs, Node *rhs) {
  Node *node = calloc(1, sizeof(Node));
  node->kind = kind;
  node->lhs = lhs;
  node->rhs = rhs;
  return node;
}

Node *new_node_num(int val) {
  Node *node = calloc(1, sizeof(Node));
  node->kind = ND_NUM;
  node->val = val;
  return node;
}

Node *primary() {
  if (consume("(")) {
    Node *node = expr();
    expect(")");
    return node;
  }
  return new_node_num(expect_number());
}

Node *unary() {
  if (consume("+")) {
    return primary();
  }
  if (consume("-")) {
    return new_node(ND_SUB, new_node_num(0), primary());
  }
  return primary();
}

Node *mul() {
  Node *node = unary();

  for (;;) {
    if (consume("*")) {
      node = new_node(ND_MUL, node, unary());
    } else if (consume("/")) {
      node = new_node(ND_DIV, node, unary());
    } else {
      return node;
    }
  }
}

Node *add() {
  Node *node = mul();

  for (;;) {
    if (consume("+")) {
      node = new_node(ND_ADD, node, mul());
    } else if (consume("-")) {
      node = new_node(ND_SUB, node, mul());
    } else {
      return node;
    }
  }
}


Node *relational() {
  Node *node = add();

  for (;;) {
    if (consume("<")) {
      node = new_node(ND_LT, node, add());
    } else if (consume("<=")) {
      node = new_node(ND_LE, node, add());
    } else if (consume(">")) {
      node = new_node(ND_LT, add(), node);
    } else if (consume(">=")) {
      node = new_node(ND_LE, add(), node);
    } else {
      return node;
    }
  }
}

Node *equality() {
  Node *node = relational();

  for (;;) {
    if (consume("==")) {
      node = new_node(ND_EQ, node, relational());
    } else if (consume("!=")) {
      node = new_node(ND_NEQ, node, relational());
    } else {
      return node;
    }
  }
}

Node *expr() {
  return equality();
}

void gen(Node *node) {
  if (node->kind == ND_NUM) {
    printf("  push %d\n", node->val);
    return;
  }

  gen(node->lhs);
  gen(node->rhs);

  printf("  pop rdi\n");
  printf("  pop rax\n");

  switch (node->kind) {
    case ND_NUM:
      break;
    case ND_ADD:
      printf("  add rax, rdi\n");
      break;
    case ND_SUB:
      printf("  sub rax, rdi\n");
      break;
    case ND_MUL:
      printf("  mul rdi\n");
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
    case ND_LE:
      printf("  cmp rax, rdi\n");
      printf("  setle al\n");
      printf("  movzb rax, al\n");
      break;
  }

  printf("  push rax\n");
}

void debug_tokenized(Token *token) {
  fprintf(stderr, "-------- tokenized --------\n");
  fprintf(stderr, "begin");
  while (token->kind != TK_EOF) {
    int kind = token->kind;
    fprintf(stderr, " -> ");
    if (kind == TK_NUM) {
      fprintf(stderr, "%d", token->val);
    } else {
      fprintf(stderr, "%s", token->str);
    }
    token = token->next;
  }
  fprintf(stderr, "\n-------- tokenized --------\n");
}

int main(int argc, char **argv) {
  if (argc != 2) {
    fprintf(stderr, "Invalid argument count\n");
    return 1;
  }
  // user input save
  user_input = argv[1];
  // tokenize
  token = tokenize(argv[1]);

  debug_tokenized(token);

  // parse
  Node *node = expr();

  // asm header
  printf(".intel_syntax noprefix\n");
  printf(".global main\n");
  printf("main:\n");

  // generate code from tree
  gen(node);

  // store result to rax
  printf("  pop rax\n");
  printf("  ret\n");
  return 0;
}
