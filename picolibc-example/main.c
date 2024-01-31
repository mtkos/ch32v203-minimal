#include <stdio.h>
#include <math.h>

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

int uart_putc(char c, FILE *file){

    REG(USART1 + DATAR) = c;
    while(!(REG(USART1 + STATR) & TC));
    return c;
}

int uart_getc(FILE *file){

    unsigned char c;

    while(!(REG(USART1 + STATR) & RXNE));
    c = REG(USART1 + DATAR);
    return c;
}

FILE __stdio = FDEV_SETUP_STREAM(uart_putc, uart_getc, NULL, _FDEV_SETUP_RW);

FILE *const stdin = &__stdio;
__strong_reference(stdin, stdout);
__strong_reference(stdin, stderr);

void uart_init(){

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

}

void main(){

    double x = 0.0;

    uart_init();

    while(1){
        getchar();
        printf("%5.1f %19.16f\r\n", x, sin(x));
        x += 0.1;
    }
}
