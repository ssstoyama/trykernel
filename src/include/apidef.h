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

/* タスクの待ち属性 */
// 待ちタスクをFIFO順で管理
#define TA_TFIFO 0x00000000
// 待ちタスクを優先度順で管理
#define TA_TPRI  0x00000001
// 待ち行列先頭のタスクを優先
#define TA_FIRST 0x00000000
// 要求数の少ないタスクを優先
#define TA_CNT   0x00000002
// 複数タスクの待ちを許さない
#define TA_WFGL  0x00000000
// 複数タスクの待ちを許す
#define TA_WMUL  0x00000008

// タスク管理API
ID tk_cre_tsk(const T_CTSK *pk_ctsk);
ER tk_sta_tsk(ID tskid, INT stacd);
void tk_ext_tsk(void);

// タスク付属同期API
ER tk_dly_tsk(RELTIM dlytim);
ER tk_slp_tsk(TMO tmout);
ER tk_wup_tsk(ID tskid);

typedef struct t_cflg {
  // イベントフラグ属性
  ATR flgatr;
  // イベントフラグ初期値
  UINT iflgptn;
} T_CFLG;

ID tk_cre_flg(const T_CFLG *pk_cflg);
ER tk_set_flg(ID flgid, UINT setptn);
ER tk_clr_flg(ID flgid, UINT clrptn);

// AND 待ち
#define TWF_ANDW   0x00000000U
// OR 待ち
#define TWF_ORW    0x00000001U
// 全ビットクリア
#define TWF_CLR    0x00000010U
// 条件ビットのみクリア
#define TWF_BITCLR 0x00000020U

ER tk_wai_flg(ID flgid, UINT waiptn, UINT wfmode, UINT *p_flgptn, TMO tmout);

/* セマフォ生成情報 */
typedef struct t_csem {
  // セマフォ属性
  ATR sematr;
  // セマフォ資源数の初期値
  INT isemcnt;
  // セマフォ資源数の最大値
  INT maxsem;
} T_CSEM;

/* セマフォAPI */
ID tk_cre_sem(const T_CSEM *pk_csem);
ER tk_sig_sem(ID semid, INT cnt);
ER tk_wai_sem(ID semid, INT cnt, TMO tmout);
