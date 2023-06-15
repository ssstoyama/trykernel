#include <trykernel.h>

TCB tcb_tbl[CNF_MAX_TSK_ID];

// タスク生成
ID tk_cre_tsk(const T_CTSK *pk_ctsk) {
  UINT intsts;
  ID tskid;
  INT i;

  // 引数チェック
  if ((pk_ctsk->tskatr & ~TA_RNG3) != (TA_HLNG | TA_USERBUF)) return E_RSATR;
  if (pk_ctsk->itskpri <= 0 || pk_ctsk->itskpri > CNF_MAX_TSK_PRI) return E_PAR;
  if (pk_ctsk->stksz == 0) return E_PAR;

  // 割り込み禁止
  DI(intsts);

  // 未使用のTCB検索
  for (i = 0; i < CNF_MAX_TSK_ID; i++) {
    if (tcb_tbl[i].state == TS_NONEXIST) break;
  }
  // TCBの初期化
  if (i < CNF_MAX_TSK_ID) {
    tcb_tbl[i].state = TS_DORMANT;
    tcb_tbl[i].pre = NULL;
    tcb_tbl[i].next = NULL;

    tcb_tbl[i].tskadr = pk_ctsk->task;
    tcb_tbl[i].itskpri = pk_ctsk->itskpri;
    tcb_tbl[i].stksz = pk_ctsk->stksz;
    tcb_tbl[i].stkadr = pk_ctsk->bufptr;

    tskid = i + 1;
  } else {
    // タスクがすでに最大数
    tskid = (ID)E_LIMIT;
  }

  // 割り込み許可
  EI(intsts);
  return tskid;
}

// タスク実行
ER tk_sta_tsk(ID tskid, INT stacd) {
  TCB *tcb;
  UINT intsts;
  ER err = E_OK;

  // 引数チェック
  if (tskid <= 0 || tskid > CNF_MAX_TSK_ID) return E_ID;
  // 割り込み禁止
  DI(intsts);

  tcb = &tcb_tbl[tskid-1];
  if (tcb->state = TS_DORMANT) {
    tcb->state = TS_READY;
    tcb->context = make_context(tcb->stkadr, tcb->stksz, tcb->tskadr);
    tqueue_add_entry(&ready_queue[tcb->itskpri], tcb);
    scheduler();
  } else {
    // タスクを実行できない
    err = E_OBJ;
  }

  // 割り込み許可
  EI(intsts);
  return err;
}

// タスク終了
void tk_ext_tsk(void) {
  UINT intsts;

  DI(intsts);

  // タスクを休止状態へ
  cur_task->state = TS_DORMANT;
  tqueue_remove_top(&ready_queue[cur_task->itskpri]);

  scheduler();
  EI(intsts);
}
