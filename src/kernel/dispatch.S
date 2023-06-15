.align 2
.thumb_func
.global DispatchEntry

// コンテキスト情報を退避する。
// DispatchEntry実行時点でマイコンの例外処理によりR0-R3,R12,LR,PC,xPSRレジスタ退避済み。
// 残りのR4-R11レジスタの内容を退避する。
DispatchEntry:
  mov     r0, #1
  // 割り込み禁止
  msr     primask, r0
  ldr     r1, =disp_running
  str     r0, [r1]

  // 1.実行中タスクのコンテキストをスタックに退避
  push    {r4-r7}
  mov     r0, r8
  mov     r1, r9
  mov     r2, r10
  mov     r3, r11
  push    {r0-r3}

  // 2.実行中タスク確認
  ldr     r0, =cur_task
  ldr     r1, [r0]
  // 現在のタスクIDが0なら最初のディスパッチ
  cmp     r1, #0
  // 現在のタスクがないためdisp_010の処理へ
  beq     disp_010

  // 3.実行コンテキストへのポインタを実行中のタスクのTCBに格納
  mov     r2, sp
  str     r2, [r1]

disp_010:
  // 4.実行中タスクの確認
  // cur_task = next_task
  ldr     r1, =sche_task
  ldr     r2, [r1]
  cmp     r2, #0
  bne     disp_030

  // 5.次に実行するタスクがなかった場合の処理
  str     r2, [r0]
disp_020:
  mov     r3, #0
  // 割り込み許可
  msr     primask, r3
  mov     r3, #1
  // 割り込み禁止
  msr     primask, r3

  ldr     r2, [r1]
  cmp     r2, #0
  beq     disp_020

  // 6.実行するタスク切り替え
disp_030:
  str     r2, [r0]
  ldr     r0, [r2]
  mov     sp, r0

  // 7.スタック上のコンテキスト復元
  pop     {r0-r3}
  mov     r11, r3
  mov     r10, r2
  mov     r9, r1
  mov     r8, r0
  pop     {r4-r7}

  // 割り込み許可
  ldr     r0, =disp_running
  mov     r1, #0
  str     r1, [r0]
  msr     primask, r1
   
  bx      lr
