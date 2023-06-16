#include <trykernel.h>

// 時間待ち状態のタスク待ち行列
TCB *wait_queue;

// システムタイマ割込みハンドラ
void systimer_handler(void) {
  TCB *tcb;

  for (tcb = wait_queue; tcb != NULL; tcb = tcb->next) {
    if (tcb->waitim == TMO_FEVR) {
      continue;
    } else if (tcb->waitim > TIMER_PERIOD) {
      // 待ち時間から経過時間を減じる。
      tcb->waitim -= TIMER_PERIOD;
    } else {
      // タスクをウェイトキューから外す
      tqueue_remove_entry(&wait_queue, tcb);
      *tcb->waierr = E_OK;
      tcb->state = TS_READY;
      tcb->waifct = TWFCT_NON;
      // タスクをレディキューにつなぐ
      tqueue_add_entry(&ready_queue[tcb->itskpri], tcb);
    }
  }
  scheduler();
}
