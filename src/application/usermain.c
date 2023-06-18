#include <trykernel.h>

ID flgid;
T_CFLG cflg = {
  // イベントフラグ属性
  .flgatr = TA_TFIFO | TA_WMUL,
  // イベントフラグ初期値
  .iflgptn = 0,
};

UB tskstk_btn[1024];
ID tskid_btn;
void task_btn(INT stacd, void *exinf);
T_CTSK ctsk_btn = {
  .tskatr = TA_HLNG | TA_RNG3 | TA_USERBUF,
  .task = task_btn,
  .itskpri = 10,
  .stksz = 1024,
  .bufptr = tskstk_btn,
};

UB tskstk_led1[1024];
ID tskid_led1;
void task_led1(INT stacd, void *exinf);
T_CTSK ctsk_led1 = {
  .tskatr = TA_HLNG | TA_RNG3 | TA_USERBUF,
  .task = task_led1,
  .itskpri = 10,
  .stksz = 1024,
  .bufptr = tskstk_led1,
};

UB tskstk_led2[1024];
ID tskid_led2;
void task_led2(INT stacd, void *exinf);
T_CTSK ctsk_led2 = {
  .tskatr = TA_HLNG | TA_RNG3 | TA_USERBUF,
  .task = task_led2,
  .itskpri = 10,
  .stksz = 1024,
  .bufptr = tskstk_led2,
};


void task_btn(INT stacd, void *exinf) {
  UW btn, btn0, diff;

  // GP13をスイッチ入力に設定
  out_w(GPIO(13), (in_w(GPIO(13)) | GPIO_PUE) & ~GPIO_PDE);
  out_w(GPIO_OE_CLR, (1 << 13));
  out_w(GPIO_CTRL(13), 5);

  // GP14をスイッチ入力に設定
  out_w(GPIO(14), (in_w(GPIO(14)) | GPIO_PUE) & ~GPIO_PDE);
  out_w(GPIO_OE_CLR, (1 << 14));
  out_w(GPIO_CTRL(14), 5);

  btn0 = in_w(GPIO_IN) & ((1 << 14) | (1 << 13));
  while (1) {
    // スイッチ読み込み
    btn = in_w(GPIO_IN) & ((1 << 14) | (1 << 13));
    if ((diff = btn ^ btn0) != 0) {
      if ((diff & (1 << 13)) && !(btn & (1 << 13))) {
        tm_putstring("BTN-1 ON\n");
        tk_set_flg(flgid, (1 << 1));
      }
      if ((diff & (1 << 14)) && !(btn & (1 << 14))) {
        tm_putstring("BTN-0 ON\n");
        tk_set_flg(flgid, (1 << 0));
      }
      btn0 = btn;
    }
    tk_dly_tsk(100);
  }
}

void task_led1(INT stacd, void *exinf) {
  UINT flgptn;

  while (1) {
    tk_wai_flg(flgid, (1 << 0), TWF_ANDW | TWF_BITCLR, &flgptn, TMO_FEVR);
    for (INT i = 0; i < 3; i++) {
      out_w(GPIO_OUT_SET, (1 << 25));
      tk_dly_tsk(500);
      out_w(GPIO_OUT_CLR, (1 << 25));
      tk_dly_tsk(500);
    }
  }
}

void task_led2(INT stacd, void *exinf) {
  UINT flgptn;

  while (1) {
    tk_wai_flg(flgid, (1 << 1), TWF_ANDW | TWF_BITCLR, &flgptn, TMO_FEVR);
    for (INT i = 0; i < 5; i++) {
      out_w(GPIO_OUT_SET, (1 << 25));
      tk_dly_tsk(100);
      out_w(GPIO_OUT_CLR, (1 << 25));
      tk_dly_tsk(100);
    }
  }
}

int usermain(void) {
  flgid = tk_cre_flg(&cflg);

  tskid_btn = tk_cre_tsk(&ctsk_btn);
  tk_sta_tsk(tskid_btn, 0);

  tskid_led1 = tk_cre_tsk(&ctsk_led1);
  tk_sta_tsk(tskid_led1, 0);

  tskid_led2 = tk_cre_tsk(&ctsk_led2);
  tk_sta_tsk(tskid_led2, 0);

  return 0;
}
