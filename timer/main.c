#include <stdint.h>

#include "../include/common.h"
#include "../include/rcc.h"
#include "../include/gpio.h"
#include "../include/timer.h"

#define PIN10 10

#define CPU_FREQ 8000000
#define CNT_MAX 0xffff
#define PRESCALER_VALUE (CPU_FREQ/CNT_MAX - 1)

void prog(){

    REG(RCC + APB2PCENR) = TIM1EN | IOPAEN;

    REG(GPIOA + CFGHR) &= Clear(PIN10 % 8);
    REG(GPIOA + CFGHR) |= Multiplexed_PP_2MHz(PIN10 % 8);

    REG(TIM1 + CCER) = CC3E; //Enable Timer 1 Channel 3 output, active high polarity
    REG(TIM1 + CHCTLR2) = OC3M_2 | OC3M_1; //PWM mode 1
    REG(TIM1 + CH3CVR) = CNT_MAX/10;

    REG(TIM1 + PSC) = PRESCALER_VALUE;
    REG(TIM1 + CTLR1) = CEN;

    REG(TIM1 + BDTR) = MOE;

    while(1){
        __asm__ volatile("wfi");
    }
}
