.set RCC, 0x40021000
.set RCC_APB2PCENR, 0x18
.set IOPBEN, 1<<3
.set GPIOB, 0x40010c00
.set CFGLR, 0x0
.set OUTDR, 0xc
.set PIN, 2
.set PIN_Clear, (~(0xf))<<(4*PIN)
.set ResetValue, 0x44444444
.set Pin_Output_PP_2MHz, 0x2<<(4*PIN)

li t0, RCC
li t1, IOPBEN
sw t1, RCC_APB2PCENR(t0)

li t0, GPIOB
li t1, (ResetValue & PIN_Clear) | Pin_Output_PP_2MHz
sw t1, CFGLR(t0)

1:
lw t1, OUTDR(t0)
xori t1, t1, 1<<PIN
sw t1, OUTDR(t0)

li t2, 1000000
2:
add t2, t2, -1
bnez t2, 2b

j 1b
