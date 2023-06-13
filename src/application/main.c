#include <trykernel.h>

static void delay_ms(UINT ms) {
  UINT cnt = ms/TIMER_PERIOD;

  while (cnt) {
    // TIMER_PERIODミリ秒ごとにSYST_CSRレジスタのCOUNTFLAGビットがセットされる
    if ((in_w(SYST_CSR) & SYST_CSR_COUNTFLAG) != 0) {
      cnt--;
    }
  }
}

int main(void) {
  tm_com_init();

  tm_putstring("Hello, World!\n");

  while (1) {
    // P25端子(内蔵LED)の出力反転
    out_w(GPIO_OUT_XOR, (1<<25));
    delay_ms(1000);
  }
  return 0;
}
