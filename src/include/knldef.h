#pragma once

#include "syslib.h"
#include "typedef.h"

typedef enum {
  TS_NONEXIST = 0, // 未登録
  TS_READY    = 1, // 実行中 or 実行可能
  TS_WAIT     = 2, // 待ち状態
  TS_DORMANT  = 3  // 休止状態
} TSTAT;

// タスク待ち要因
typedef enum {
  // 無し
  TWFCT_NON = 0,
  // tk_dly_tskによる時間待ち
  TWFCT_DLY = 1,
  // tk_slp_tskによる起床待ち
  TWFCT_SLP = 2,
  // tk_wai_flgによるフラグ待ち
  TWFCT_FLG = 3,
  // tk_wai_semによるセマフォ待ち
  TWFCT_SEM = 4,
} TWFCT;

typedef struct st_tcb {
  // コンテキストへのポインタ
  void *context;

  struct st_tcb *pre;
  struct st_tcb *next;

 /* タスク情報 */
  TSTAT state;
  // 実行開始アドレス
  FP tskadr;
  // 実行優先度
  PRI itskpri;
  // スタックアドレス
  void *stkadr;
  // スタックサイズ
  SZ stksz;
  // 起床要求数
  INT wupcnt;

  /* 時間待ち情報 */
  // 待ち要因
  TWFCT waifct;
  // 待ち時間
  RELTIM waitim;
  // 待ち解除のエラーコード
  ER *waierr;

  /* イベントフラグ待ち情報 */
  // 待ちフラグパターン
  UINT waiptn;
  // 待ちモード
  UINT wfmode;
  // 待ち解除時のフラグパターン
  UINT *p_flgptn;

  // セマフォ資源要求数
  INT waisem;
} TCB;

// TCBテーブル
extern TCB tcb_tbl[];
// タスク実行待ち行列
extern TCB *ready_queue[];
// 実行中タスク
extern TCB *cur_task;
// 次に実行するタスク
extern TCB *sche_task;
// タスクの時間待ち行列
extern TCB *wait_queue;

// リセットハンドラ
extern void ResetHandler(void);
// ディスパッチャ
extern void DispatchEntry(void);
// システムタイマ割込みハンドラ
extern void systimer_handler(void);

// 割り込み制御ステートレジスタのアドレス
#define SCB_ICSR 0xE000ED04
// PendSV set-pending ビット
#define ICSR_PENDSVSET (1<<28)
static inline void dispatch(void) {
  out_w(SCB_ICSR, ICSR_PENDSVSET);
}

extern void scheduler(void);

extern void *make_context(UW *sp, UINT ssize, void (*fp)());

extern void tqueue_add_entry(TCB **queue, TCB *tcb);
extern void tqueue_remove_top(TCB **queue);
extern void tqueue_remove_entry(TCB **queue, TCB *tcb);

typedef enum {
  // 未登録
  KS_NONEXIST = 0,
  // 登録済み
  KS_EXIST    = 1,
} KSSTAT;

typedef struct st_flgcb {
  // イベントフラグ状態
  TSTAT state;
  // イベントフラグ値
  UINT flgptn;
} FLGCB;

typedef struct semaphore_control_block {
  KSSTAT state;
  INT semcnt;
  INT maxsem;
} SEMCB;

// カーネルメイン
extern int main(void);

extern int usermain(void);
