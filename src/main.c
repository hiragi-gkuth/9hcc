#include <stdio.h>
#include "9hcc.h"


Token *token;
Node *code[100];
char *user_input;

int main(int argc, char **argv) {
  if (argc != 2) {
    fprintf(stderr, "Invalid argument count\n");
    return 1;
  }
  // user input save
  user_input = argv[1];
  // tokenize
  tokenize(argv[1]);
  // debug_tokenized(token);
  // construct node
  program();

  // asm header
  printf(".intel_syntax noprefix\n");
  printf(".global main\n");
  printf("main:\n");

  // asm prologue
  printf("  push rbp\n");
  printf("  mov rbp, rsp\n");
  printf("  sub rsp, 208\n"); // 8byte * 26 idents

  // gen code
  for (int i = 0; code[i]; i++) {
    gen(code[i]);

    // pop result by code
    printf("  pop rax\n");
  }

  // epilogue
  printf("  mov rsp, rbp\n");
  printf("  pop rax\n");
  printf("  ret\n");
  return 0;
}