#include <stdint.h>

#include "../include/common.h"
#include "../include/pfic.h"
#include "../include/rcc.h"
#include "../include/gpio.h"
#include "../include/exti.h"
#include "../include/afio.h"

#define EXTI1_IRQn 23

#define PIN1 1
#define PIN2 2

void EXTI1_IRQHandler() __attribute__((interrupt));

void EXTI1_IRQHandler(){
    REG(EXTI + INTFR) = Pin(PIN1);
}

void prog(){

    REG(RCC + APB2PCENR) = IOPBEN | AFIOEN;

    REG(GPIOB + CFGLR) &= Clear(PIN1);
    REG(GPIOB + CFGLR) |= Input_PUPD(PIN1);

    REG(GPIOB + CFGLR) &= Clear(PIN2);
    REG(GPIOB + CFGLR) |= Output_PP_2MHz(PIN2);

    REG(AFIO + EXTICR1) = PB_Exti(PIN1);
    REG(EXTI + INTENR) = Pin(PIN1);
    REG(EXTI + FTENR) = Pin(PIN1);  // falling edge trigger

    REG(PFIC + IENR0) = 1<<EXTI1_IRQn;

    while(1){
        __asm__ volatile("wfi");
        REG(GPIOB + OUTDR) ^= Pin(PIN2);
    }
}
