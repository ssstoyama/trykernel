#include <trykernel.h>

// タスク1スタック
UB tskstk_1[1024];
// タスク1ID
ID tskid_1;

// タスク1生成情報
void task_1(INT stacd, void *exinf);
T_CTSK ctsk_1 = {
  .tskatr = TA_HLNG | TA_RNG3 | TA_USERBUF,
  .task = task_1,
  .itskpri = 10,
  .stksz = 1024,
  .bufptr = tskstk_1,
};

UB tskstk_2[1024];
ID tskid_2;

void task_2(INT stacd, void *exinf);
T_CTSK ctsk_2 = {
  .tskatr = TA_HLNG | TA_RNG3 | TA_USERBUF,
  .task = task_2,
  .itskpri = 10,
  .stksz = 1024,
  .bufptr = tskstk_2,
};

// タスク1実行関数
void task_1(INT stacd, void *exinf) {
  while (1) {
    out_w(GPIO_OUT_XOR, (1<<25));
    tk_dly_tsk(1000);
  }
}

// タスク2実行関数
void task_2(INT stacd, void *exinf) {
  while (1) {
    tm_putstring("Hello\n");
    tk_dly_tsk(1000);
  }
}

int usermain(void) {
  tm_putstring("Start user-main\n");

  tskid_1 = tk_cre_tsk(&ctsk_1);
  tk_sta_tsk(tskid_1, 0);

  tskid_2 = tk_cre_tsk(&ctsk_2);
  tk_sta_tsk(tskid_2, 0);

  return 0;
}
