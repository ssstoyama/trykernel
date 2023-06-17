#include <trykernel.h>

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

UB tskstk_led[1024];
ID tskid_led;
void task_led(INT stacd, void *exinf);
T_CTSK ctsk_led = {
  .tskatr = TA_HLNG | TA_RNG3 | TA_USERBUF,
  .task = task_led,
  .itskpri = 10,
  .stksz = 1024,
  .bufptr = tskstk_led,
};

void task_btn(INT stacd, void *exinf) {
  UW btn, btn0;

  out_w(GPIO(14), (in_w(GPIO(14)) | GPIO_PUE) & ~GPIO_PDE);
  out_w(GPIO_OE_CLR, (1 << 14));
  out_w(GPIO_CTRL(14), 5);

  btn0 = in_w(GPIO_IN) & (1 << 14);
  while (1) {
    // スイッチ読み込み
    btn = in_w(GPIO_IN) & (1 << 14);
    if (btn != btn0) {
      if (btn == 0) {
        tm_putstring("BTN ON\n");
        tk_wup_tsk(tskid_led);
      }
      btn0 = btn;
    }
    tk_dly_tsk(100);
  }
}

void task_led(INT stacd, void *exinf) {
  while (1) {
    tk_slp_tsk(TMO_FEVR);
    out_w(GPIO_OUT_SET, (1 << 25));
    tk_dly_tsk(1000);
    out_w(GPIO_OUT_CLR, (1 << 25));
  }
}

int usermain(void) {
  tskid_btn = tk_cre_tsk(&ctsk_btn);
  tk_sta_tsk(tskid_btn, 0);

  tskid_led = tk_cre_tsk(&ctsk_led);
  tk_sta_tsk(tskid_led, 0);

  return 0;
}
