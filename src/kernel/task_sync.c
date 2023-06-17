#include <trykernel.h>

ER tk_dly_tsk(RELTIM dlytim) {
  UINT intsts;
  ER err = E_OK;

  DI(intsts);
  if (dlytim > 0) {
    // タスクをレディキューから外す
    tqueue_remove_top(&ready_queue[cur_task->itskpri]);

    // TCBの情報変更
    cur_task->state = TS_WAIT;
    cur_task->waifct = TWFCT_DLY;
    cur_task->waitim = dlytim + TIMER_PERIOD;
    cur_task->waierr = &err;

    tqueue_add_entry(&wait_queue, cur_task);
    scheduler();
  }
  EI(intsts);
  return err;
}

// タスク起床待ちAPI
ER tk_slp_tsk(TMO tmout) {
  UINT intsts;
  ER err = E_OK;

  DI(intsts);
  if (cur_task->wupcnt > 0) {
    // 起床要求あり
    cur_task->wupcnt--;
  } else {
    // タスクをレディキューから外す
    tqueue_remove_top(&ready_queue[cur_task->itskpri]);

    cur_task->state = TS_WAIT;
    cur_task->waifct = TWFCT_SLP;
    cur_task->waitim = (tmout == TMO_FEVR) ? tmout : (tmout + TIMER_PERIOD);
    cur_task->waierr = &err;

    // タスクをウェイトキューに繋ぐ
    tqueue_add_entry(&wait_queue, cur_task);
    scheduler();
  }
  // 割り込み許可
  EI(intsts);
  return err;
}

ER tk_wup_tsk(ID tskid) {
  TCB *tcb;
  UINT intsts;
  ER err = E_OK;

  if (tskid <= 0 || tskid > CNF_MAX_TSK_ID) return E_ID;

  DI(intsts);
  tcb = &tcb_tbl[tskid - 1];
  // スリープ状態のタスクか
  if ((tcb->state == TS_WAIT) && (tcb->waifct == TWFCT_SLP)) {
    tqueue_remove_entry(&wait_queue, tcb);

    tcb->state = TS_READY;
    tcb->waifct = TWFCT_NON;

    tqueue_add_entry(&ready_queue[tcb->itskpri], tcb);
    scheduler();
  } else if (tcb->state == TS_READY || tcb->state == TS_WAIT) {
    tcb->wupcnt++;
  } else {
    err = E_OBJ;
  }

  EI(intsts);
  return err;
}
