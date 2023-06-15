#include <trykernel.h>

void tqueue_add_entry(TCB **queue, TCB *tcb) {
  TCB *queue_end;

  if (*queue == NULL) {
    // キューが空なので先頭に追加
    *queue = tcb;
    tcb->pre = tcb;
  } else {
    // キューの終端に追加
    queue_end = (*queue)->pre;
    queue_end->next = tcb;
    tcb->pre = queue_end;
    (*queue)->pre = tcb;
  }
  tcb->next = NULL;
}

void tqueue_remove_top(TCB **queue) {
  TCB *top;

  if (*queue == NULL) return;  // キューは空

  top = *queue;
  *queue = top->next;
  if (*queue != NULL) {
    (*queue)->pre = top->pre;
  }
}

void tqueue_remove_entry(TCB **queue, TCB *tcb) {
  if (*queue == tcb) {
    // 指定したエントリがキューの先頭の場合
    tqueue_remove_top(queue);
  } else {
    (tcb->pre)->next = tcb->next;
    if (tcb->next != NULL) {
      (tcb->next)->pre = tcb->pre;
    } else {
      (*queue)->pre = tcb->pre;
    }
  }
}
