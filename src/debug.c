#include <stdio.h>
#include "9hcc.h"

void debug_tokenized() {
  fprintf(stderr, "-------- tokenized --------\n");
  fprintf(stderr, "begin");
  Token *seek = token;
  
  while (seek->kind != TK_EOF) {
    int kind = seek->kind;
    fprintf(stderr, " -> ");
    if (kind == TK_NUM) {
      fprintf(stderr, "\'%d\'", seek->val);
    } else {
      fprintf(stderr, "\'%.*s\'", seek->len, seek->str);
    }
    seek = seek->next;
  }
  fprintf(stderr, " -> EOF\n");
  fprintf(stderr, "\n----------- end -----------\n");
}
