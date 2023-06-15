#include <trykernel.h>

// タスクのレディキュー
TCB *ready_queue[CNF_MAX_TSK_PRI];
// 実行中タスク
TCB *cur_task;
// 次に実行するタスク
TCB *sche_task;

// ディスパッチャ実行中
UW disp_running;

void scheduler(void) {
  INT i;

  for (i = 0; i < CNF_MAX_TSK_PRI; i++) {
    if (ready_queue[i] != NULL) break;
  }

  if (i < CNF_MAX_TSK_PRI) {
    sche_task = ready_queue[i];
  } else {
    // 実行できるタスクなし
    sche_task = NULL;
  }
  if (sche_task != cur_task && !disp_running) {
    // ディスパッチャを実行
    dispatch();
  }
}
