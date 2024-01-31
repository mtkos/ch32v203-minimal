#define AFIO 0x40010000
#define   PCFR1 0x4
#define   EXTICR1 0x8
#define   EXTICR4 0x14
#define     PB_Exti(x) (0b0001<<(4*(x)))
#define     PC_Exti(x) (0b0010<<(4*(x)))
