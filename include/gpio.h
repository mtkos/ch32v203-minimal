#define GPIOA 0x40010800
#define GPIOB 0x40010c00
#define GPIOC 0x40011000
#define GPIOD 0x40011400
#define   CFGLR 0x0
#define   CFGHR 0x4
#define     Clear(x) (~(0b1111 << 4*(x)))
#define     Input_PUPD(x) ((uint32_t)0b1000 << 4*(x))
#define     Output_PP_2MHz(x) (0b0010 << 4*(x))
#define     Output_PP_10MHz(x) (0b0001 << 4*(x))
#define     Output_PP_50MHz(x) (0b0011 << 4*(x))
#define     Multiplexed_PP_2MHz(x) (0b1010 << 4*(x))
#define     Input_Floating(x) (0b0100 << 4*(x))
#define   OUTDR 0xc
#define     Pin(x) (1<<(x))
#define   BSHR 0x10


