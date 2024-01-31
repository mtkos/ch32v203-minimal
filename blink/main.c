#include <stdint.h>

#include "../include/common.h"
#include "../include/rcc.h"
#include "../include/gpio.h"

#define PIN1 1
#define PIN2 2

uint32_t count = 1000;

extern uint32_t _sidata, _sdata, _edata, _ebss, _sbss;

uint32_t period(){

    count += 1000;
    return count;
}

void flash_to_ram(){

    uint32_t *p_idata = &_sidata;

    for(uint32_t *p_data = &_sdata; p_data < &_edata; *p_data++ = *p_idata++);
    for(uint32_t *p_bss = &_sbss; p_bss < &_ebss; *p_bss++ = 0);

}

void prog(){

    flash_to_ram();

    REG(RCC + APB2PCENR) |= IOPBEN;

    REG(GPIOB + CFGLR) &= Clear(PIN1);
    REG(GPIOB + CFGLR) |= Output_PP_2MHz(PIN1);
    REG(GPIOB + CFGLR) &= Clear(PIN2);
    REG(GPIOB + CFGLR) |= Output_PP_2MHz(PIN2);

    REG(GPIOB + OUTDR) = Pin(PIN1);
    while(1){
        for(volatile uint32_t i = period(); i > 0; i--);
        REG(GPIOB + OUTDR) ^= (Pin(PIN1) | Pin(PIN2));
    }
}
