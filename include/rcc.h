#define RCC 0x40021000
#define   CTLR 0x0
#define     PLLRDY 1<<25
#define     PLLON 1<<24
#define   CFGR0 0x4
#define     HPRE_Reset_Mask 0xFFFFFF0F
#define     HPRE_DIV1 0
#define     PLL_MUL18 (0b1111 << 18)
#define     SW_PLL 0x2
#define   APB2PCENR 0x18
#define     USART1EN 1<<14
#define     TIM1EN 1<<11
#define     IOPDEN 1<<5
#define     IOPCEN 1<<4
#define     IOPBEN 1<<3
#define     IOPAEN 1<<2
#define     AFIOEN 1<<0

