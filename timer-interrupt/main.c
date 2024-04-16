#include <stdint.h>

#include "../include/common.h"
#include "../include/pfic.h"
#include "../include/rcc.h"
#include "../include/gpio.h"
#include "../include/timer.h"

#define TIM1_UP_IRQn 41

#define PIN2 2

#define CPU_FREQ 8000000
#define CNT_MAX 0xffff
#define PRESCALER_VALUE (CPU_FREQ/CNT_MAX - 1)

void TIM1_UP_IRQHandler() __attribute__((interrupt));

void TIM1_UP_IRQHandler(){
    REG(TIM1 + INTFR) = 0;
}

void prog(){

    REG(RCC + APB2PCENR) = TIM1EN | IOPBEN;

    REG(GPIOB + CFGLR) &= Clear(PIN2);
    REG(GPIOB + CFGLR) |= Output_PP_2MHz(PIN2);

    REG(TIM1 + PSC) = PRESCALER_VALUE;

    REG(TIM1 + DMAINTENR) = UIE;
    REG(PFIC + IENR1) = (1<<(TIM1_UP_IRQn % 32));
    REG(PFIC + VTFIDR) = TIM1_UP_IRQn;
    REG(PFIC + VTFADDRR0) = (uint32_t)TIM1_UP_IRQHandler + 1;

    REG(TIM1 + CTLR1) = CEN;

    while(1){
        __asm__ volatile ("wfi");
        REG(GPIOB + OUTDR) ^= Pin(PIN2);
    }
}
