#include <stdio.h>
#include "9hcc.h"

void debug_tokenized(Token *token) {
  fprintf(stderr, "-------- tokenized --------\n");
  fprintf(stderr, "begin");
  while (token->kind != TK_EOF) {
    int kind = token->kind;
    fprintf(stderr, " -> ");
    if (kind == TK_NUM) {
      fprintf(stderr, "\'%d\'", token->val);
    } else {
      fprintf(stderr, "\'%s\'", token->str);
    }
    token = token->next;
  }
  fprintf(stderr, "\n-------- tokenized --------\n");
}

