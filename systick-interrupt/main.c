#include <stdint.h>

#include "../include/common.h"
#include "../include/pfic.h"
#include "../include/systick.h"
#include "../include/rcc.h"
#include "../include/gpio.h"

#define SysTicK_IRQn 12

#define PIN1 1
#define PIN2 2

volatile uint8_t onoff[3];
volatile uint32_t period;

extern uint32_t _sidata, _sdata, _edata, _ebss, _sbss;

void SysTick_Handler() __attribute__((interrupt));

void SysTick_Handler(){

    REG(GPIOB + BSHR) = Pin(PIN1 + 16*onoff[1]);
    REG(SysTick + SR) = 0;
    onoff[1] = 1 - onoff[1];
}

void flash_to_ram(){

    uint32_t *p_idata = &_sidata;

    for(uint32_t *p_data = &_sdata; p_data < &_edata; *p_data++ = *p_idata++);
    for(uint32_t *p_bss = &_sbss; p_bss < &_ebss; *p_bss++ = 0);

}

void inc(){

    period += 1000;
}

void prog(){

    flash_to_ram();

    REG(RCC + APB2PCENR) |= IOPBEN;

    REG(GPIOB + CFGLR) &= Clear(PIN1);
    REG(GPIOB + CFGLR) |= Output_PP_2MHz(PIN1);
    REG(GPIOB + CFGLR) &= Clear(PIN2);
    REG(GPIOB + CFGLR) |= Output_PP_2MHz(PIN2);

    REG(SysTick + CMPLR) = 1000000;
    REG(SysTick + CTLR) = STE | STIE | STRE;

    REG(PFIC + IENR0) = 1<<SysTicK_IRQn;

    while(1){
        for(volatile uint32_t i = period; i > 0; i--);
        REG(GPIOB + BSHR) = Pin(PIN2 + 16*onoff[2]);
        onoff[2] = 1 - onoff[2];
        inc();
    }
}
