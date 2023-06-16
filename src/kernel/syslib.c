#include <trykernel.h>

// UART0の初期化
void tm_com_init(void) {
  // ボーレート設定
  out_w(UART0_BASE+UARTx_IBRD, 67);
  out_w(UART0_BASE+UARTx_FBRD, 52);
  // データ形式設定
  out_w(UART0_BASE+UARTx_LCR_H, 0x70);
  // 通信有効化
  out_w(UART0_BASE+UARTx_CR, UART_CR_RXE | UART_CR_TXE | UART_CR_EN);
}

// デバッグ用UART出力
UINT tm_putstring(char* str) {
  UINT cnt = 0;
  while(*str) {
    // 送信FIFOの空き待ち
    while((in_w(UART0_BASE+UARTx_FR) & UART_FR_TXFF) != 0);
    out_w(UART0_BASE+UARTx_DR, *str++);
    cnt++;
  }
  return cnt;
}

// 時間待ち関数 
void delay_ms(UINT ms) {
  UINT cnt = ms/TIMER_PERIOD;

  while(cnt) {
    // TIMER_PERIOD経過するとフラグがセット
    if((in_w(SYST_CSR) & SYST_CSR_COUNTFLAG)!=0) {
      cnt--;
    }
  }
}
