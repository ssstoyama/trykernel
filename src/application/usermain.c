#include <trykernel.h>

// タスク0スタック
UB tskstk_0[1024];
// タスク0ID
ID tskid_0;

// タスク0生成情報
void task_0(INT stacd, void *exinf);
T_CTSK ctsk_0 = {
  .tskatr = TA_HLNG | TA_RNG3 | TA_USERBUF,
  .task = task_0,
  .itskpri = 15,
  .stksz = 1024,
  .bufptr = tskstk_0,
};

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

// タスク0実行関数
void task_0(INT stacd, void *exinf) {
  tm_putstring("Start Task-0\n");

  int selected = 0;
  tskid_1 = tk_cre_tsk(&ctsk_1);
  tskid_2 = tk_cre_tsk(&ctsk_2);

  while(1) {
    switch (selected) {
    case 0:
      tk_sta_tsk(tskid_1, 0);
      break;
    case 1:
      tk_sta_tsk(tskid_2, 0);
      break;
    }
    selected = ++selected % 2;
    delay_ms(500);
    scheduler();
  }
}

// タスク1実行関数
void task_1(INT stacd, void *exinf) {
  tm_putstring("Start Task-1\n");
  // LED on
  out_w(GPIO_OUT_SET, (1<<25));
  tk_ext_tsk();
}

// タスク2実行関数
void task_2(INT stacd, void *exinf) {
  tm_putstring("Start Task-2\n");
  // LED off
  out_w(GPIO_OUT_CLR, (1<<25));
  tk_ext_tsk();
}

int usermain(void) {
  tm_putstring("Start user-main\n");

  tskid_0 = tk_cre_tsk(&ctsk_0);

  tk_sta_tsk(tskid_0, 0);

  return 0;
}
