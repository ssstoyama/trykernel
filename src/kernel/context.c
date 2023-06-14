#include <trykernel.h>

typedef struct {
  // R4-R11レジスタ
  UW r_[8];
  // R0-R3レジスタ
  UW r[4];
  // R12レジスタ
  UW ip;
  // lrレジスタ
  UW lr;
  // pcレジスタ
  UW pc;
  // xpsrレジスタ
  UW xpsr;
} StackFrame;

void *make_context(UW *sp, UINT ssize, void (*fp)()) {
  StackFrame *sfp;

  // スタック上の実行コンテキスト情報のポインタセット
  sfp = (StackFrame*)((UW)sp + ssize);
  sfp--;

  // 実行コンテキスト初期化
  sfp->xpsr = 0x01000000;
  sfp->pc = (UW)fp & ~0x00000001UL;

  return (void*)sfp;
}
