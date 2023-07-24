#include "9cc.h"

int main(int argc, char **argv) {
  if (argc != 2) {
    error("%s: 引数の数が正しくありません", argv[0]);
    return 1;
  }

  user_input = argv[1];
  token = tokenize();
  Function *prog = program();

  for (Function *fn = prog; fn; fn = fn->next) {
    int offset = 0;
    for(VarList *vl = fn->locals; vl; vl = vl->next) {
      // 8 でいいの？？
      offset += 8;
      vl->var->offset = offset;
    }
    fn->stack_size = offset;
  }
  codegen(prog);

  return 0;
}