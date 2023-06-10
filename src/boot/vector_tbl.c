/*
 * 例外ベクタテーブル
 */

#include "typedef.h"
#include "sysdef.h"
#include "syslib.h"
#include "knldef.h"

// デフォルトハンドラ
void DefaultHandler(void) {
  while (1);
}

void (* const vector_tbl[])() __attribute__((section(".vector"))) = {
  (void(*)()) (INITIAL_SP), // 0: スタックポインタ初期値。リセットハンドラ実行中のみ使用する。
  ResetHandler,                        // 1: Reset
  DefaultHandler,          // 2: NMI
  DefaultHandler,          // 3: Hard Fault
  0,                        // 4: 未使用
  0,                        // 5: 未使用
  0,                        // 6: 未使用
  0,                        // 7: 未使用
  0,                        // 8: 未使用
  0,                        // 9: 未使用
  0,                        // 10: 未使用
  DefaultHandler,          // 11: Svcall
  0,                        // 12: 未使用
  0,                        // 13: 未使用
  DefaultHandler,          // 14: PendSV
  DefaultHandler,          // 15: Systick
  DefaultHandler,          // IRQ 0
  DefaultHandler,          // IRQ 1
  DefaultHandler,          // IRQ 2
  DefaultHandler,          // IRQ 3
  DefaultHandler,          // IRQ 4
  DefaultHandler,          // IRQ 5
  DefaultHandler,          // IRQ 6
  DefaultHandler,          // IRQ 7
  DefaultHandler,          // IRQ 8
  DefaultHandler,          // IRQ 9
  DefaultHandler,          // IRQ 10
  DefaultHandler,          // IRQ 11
  DefaultHandler,          // IRQ 12
  DefaultHandler,          // IRQ 13
  DefaultHandler,          // IRQ 14
  DefaultHandler,          // IRQ 15
  DefaultHandler,          // IRQ 16
  DefaultHandler,          // IRQ 17
  DefaultHandler,          // IRQ 18
  DefaultHandler,          // IRQ 19
  DefaultHandler,          // IRQ 20
  DefaultHandler,          // IRQ 21
  DefaultHandler,          // IRQ 22
  DefaultHandler,          // IRQ 23
  DefaultHandler,          // IRQ 24
  DefaultHandler,          // IRQ 25
  DefaultHandler,          // IRQ 26
  DefaultHandler,          // IRQ 27
  DefaultHandler,          // IRQ 28
  DefaultHandler,          // IRQ 29
  DefaultHandler,          // IRQ 30
  DefaultHandler,          // IRQ 31
};
