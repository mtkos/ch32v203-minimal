#include <stdint.h>

#include "../include/common.h"
#include "../include/rcc.h"
#include "../include/gpio.h"

#define PIN 2

void prog(){

    REG(RCC + APB2PCENR) = IOPBEN;

    REG(GPIOB + CFGLR) &= Clear(PIN);
    REG(GPIOB + CFGLR) |= Output_PP_2MHz(PIN);

    while(1){
        for(volatile uint32_t i = 100000; i > 0; i--);
        REG(GPIOB + OUTDR) ^= Pin(PIN);
    }
}
