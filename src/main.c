#include <stdio.h>
#include "9hcc.h"

Token *token;
char *user_input;

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