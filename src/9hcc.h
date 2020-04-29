#include <stdbool.h>

// error.c
void error_at(char *loc, char *fmt, ...);
void error(char *fmt, ...);

// tokenize.c
void tokenize(char *p);
typedef enum {
  TK_RESERVED,
  TK_IDENT,
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

Token *consume_ident();
bool consume(char* op);
void expect(char *op);
int expect_number();
bool at_eof();


// codegen.c
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
  ND_ASSIGN, // =
  ND_LVAR // local variable
} NodeKind;

typedef struct Node Node;

struct Node {
  NodeKind kind;
  Node *lhs;
  Node *rhs;
  int val;    // use for ND_NUM
  int offset; // use for ND_LVAR
};

// parse.c
// Construct Nodes
void program();
// Generate assembly from Nodes
void gen(Node *node);

// debug.c
void debug_tokenized();

// externs
extern Token *token;
extern Node *code[];
extern char *user_input;