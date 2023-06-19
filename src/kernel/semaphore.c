#include <trykernel.h>

SEMCB semcb_tbl[CNF_MAX_SEM_ID];

// セマフォ生成API
ID tk_cre_sem(const T_CSEM *pk_csem) {
  ID semid;
  UINT intsts;

  DI(intsts);
  for (semid = 0; semcb_tbl[semid].state != KS_NONEXIST; semid++);

  if (semid < CNF_MAX_SEM_ID) {
    semcb_tbl[semid].state = KS_EXIST;
    semcb_tbl[semid].semcnt = pk_csem->isemcnt;
    semcb_tbl[semid].maxsem = pk_csem->maxsem;
    semid++;
  } else {
    semid = E_LIMIT;
  }

  EI(intsts);
  return semid;
}

// セマフォ資源獲得API
ER tk_wai_sem(ID semid, INT cnt, TMO tmout) {
  SEMCB *semcb;
  ER err = E_OK;
  UINT intsts;

  if (semid <= 0 || semid > CNF_MAX_SEM_ID) return E_ID;

  DI(intsts);
  semcb = &semcb_tbl[--semid];
  if (semcb->state == KS_EXIST) {
    if (semcb->semcnt >= cnt) {
      // 現在のセマフォの資源数 >= 要求する資源数
      semcb->semcnt -= cnt;
    } else if (tmout == TMO_POL) {
      // 資源が足りなく、かつ、待ち時間0の場合
      err = E_TMOUT;
    } else {
      // 資源が足りなく、待ち状態に以降
      tqueue_remove_top(&ready_queue[cur_task->itskpri]);

      cur_task->state = TS_WAIT; // タスクを待ち状態へ
      cur_task->waifct = TWFCT_SEM; // 待ち要因
      cur_task->waitim = ((tmout == TMO_FEVR) ? tmout : tmout + TIMER_PERIOD); // 待ち時間
      cur_task->waisem = cnt;
      cur_task->waierr = &err;

      tqueue_add_entry(&wait_queue, cur_task);
      scheduler();
    }
  } else {
    err = E_NOEXS;
  }
  EI(intsts);
  return err;
}

// セマフォ資源返却API
ER tk_sig_sem(ID semid, INT cnt) {
  SEMCB *semcb;
  TCB *tcb;
  ER err = E_OK;
  UINT intsts;

  if (semid <= 0 || semid > CNF_MAX_SEM_ID) return E_ID;

  DI(intsts);
  semcb = &semcb_tbl[--semid];
  if (semcb->state == KS_EXIST) {
    // 資源返却
    semcb->semcnt += cnt;
    if (semcb->semcnt <= semcb->maxsem) {
      for (tcb = wait_queue; tcb != NULL; tcb = tcb->next) {
        if(tcb->waifct == TWFCT_SEM) {
          if (semcb->semcnt >= tcb->waisem) {
            // 要求資源数を満たしていれば実行可能状態へ
            semcb->semcnt -= tcb->waisem;
            tqueue_remove_entry(&wait_queue, tcb);

            tcb->state = TS_READY;
            tcb->waifct = TWFCT_NON;
            tcb->waierr = &err;

            tqueue_add_entry(&ready_queue[tcb->itskpri], tcb);
            scheduler();
          }
        } else {
          break;
        }
      }
    } else {
      // 資源数が最大数を超えた
      semcb->semcnt -= cnt;
      err = E_QOVR;
    }
  } else {
    err = E_NOEXS;
  }
  EI(intsts);
  return err;
}
