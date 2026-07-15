.set USB_LP_CAN1_RX0_IRQn, 36
.set MIE, 1<<3
.set MODE1, 1<<1
.set MODE0, 1<<0

.option norvc
.section .init
  j      Reset_Handler
.option rvc
.skip    4*(USB_LP_CAN1_RX0_IRQn - 1)
.word    USB_LP_CAN1_RX0_IRQ_Handler

Reset_Handler:
.option norelax
  la gp, _global_ptr
.option relax

  la sp, _estack

  li t0, MODE1 | MODE0
  csrw mtvec, t0

  li t0, MIE
  csrw mstatus, t0

  jal prog
