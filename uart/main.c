#include <stdint.h>

#include "../include/common.h"
#include "../include/rcc.h"
#include "../include/gpio.h"
#include "../include/uart.h"
#include "../include/exten-ctr.h"

#define BAUDRATE 921600
#define CLOCK_FREQUENCY 144000000
#define BAUD_REG (CLOCK_FREQUENCY/BAUDRATE - 1)

#define PIN9 1
#define PIN10 2

int prog(){

    REG(RCC + CFGR0) &= HPRE_Reset_Mask;
    REG(RCC + CFGR0) |= HPRE_DIV1;

    REG(EXTEN_CTR) |= HSIPRE;
    REG(RCC + CFGR0) |= PLL_MUL18;
    REG(RCC + CTLR) |= PLLON;
    while(!(REG(RCC + CTLR) & PLLRDY));
    REG(RCC + CFGR0) |= SW_PLL;

    REG(RCC + APB2PCENR) = USART1EN | IOPAEN;

    //PA9 = TX
    REG(GPIOA + CFGHR) &= Clear(PIN9);
    REG(GPIOA + CFGHR) |= Multiplexed_PP_2MHz(PIN9);

    //PA10 = RX
    REG(GPIOA + CFGHR) &= Clear(PIN10);
    REG(GPIOA + CFGHR) |= Input_PUPD(PIN10);

    REG(USART1 + BRR) = BAUD_REG;
    REG(USART1 + CTLR1) = TE | RE | UE;

    while(1){
        while(!(REG(USART1 + STATR) & RXNE));
        REG(USART1 + DATAR)++;
        while(!(REG(USART1 + STATR) & TC));
    }
}
