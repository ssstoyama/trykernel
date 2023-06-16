#pragma once

#include "typedef.h"

// タイムアウト時間 0
#define TMO_POL  (0)
// 無限待ち
#define TMO_FEVR (-1)

// タスク生成情報
typedef struct {
  // タスク属性
  ATR tskatr;
  // タスク起動アドレス
  FP task;
  // タスク優先度
  PRI itskpri;
  // スタックサイズ
  SZ stksz;
  // スタックのバッファポインタ
  void *bufptr;
} T_CTSK;

// タスク属性
#define TA_HLNG     0x0000001
#define TA_USERBUF  0x0000020
#define TA_RNG0     0x0000000
#define TA_RNG1     0x0000100
#define TA_RNG2     0x0000200
#define TA_RNG3     0x0000300

// タスク管理API
ID tk_cre_tsk(const T_CTSK *pk_ctsk);
ER tk_sta_tsk(ID tskid, INT stacd);
void tk_ext_tsk(void);

// タスク付属同期API
ER tk_dly_tsk(RELTIM dlytim);
