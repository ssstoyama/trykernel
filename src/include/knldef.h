#pragma once

#include "syslib.h"
#include "typedef.h"

// リセットハンドラ
extern void ResetHandler(void);
extern void DispatchEntry(void);

// 割り込み制御ステートレジスタのアドレス
#define SCB_ICSR 0xE000ED04
// PendSV set-pending ビット
#define ICSR_PENDSVSET (1<<28)
static inline void dispatch(void) {
  out_w(SCB_ICSR, ICSR_PENDSVSET);
}

extern void scheduler(void);

extern void *make_context(UW *sp, UINT ssize, void (*fp)());

// カーネルメイン
extern int main(void);
