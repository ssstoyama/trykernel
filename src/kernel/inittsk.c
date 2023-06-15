#include <trykernel.h>

void initsk(INT stacd, void *exinf);
UB taskstk_ini[256];
ID tskid_ini;

T_CTSK ctsk_init = {
  .tskatr = TA_HLNG | TA_RNG0 | TA_USERBUF,
  .task = initsk,
  .itskpri = 1, // 優先度MAX
  .stksz = sizeof(taskstk_ini),
  .bufptr = taskstk_ini,
};

void initsk(INT stacd, void *exinf) {
  tm_com_init();
  tm_putstring("Start Try Kernel\n");

  usermain();
  tk_ext_tsk();
}

int main(void) {
  tskid_ini = tk_cre_tsk(&ctsk_init);
  tk_sta_tsk(tskid_ini, 0);

  while (1);
}
