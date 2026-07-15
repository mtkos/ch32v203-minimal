#include <stdint.h>

#include "../include/common.h"
#include "../include/rcc.h"
#include "../include/pfic.h"
#include "../include/exten-ctr.h"
#include "../include/usb.h"
#include "descriptors.h"

#define USB_LP_CAN1_RX0_IRQn 36

struct{
    uint8_t bmRequestType;
    uint8_t bRequest;
    uint8_t wValue_low;
    uint8_t wValue_high;
    uint16_t wIndex;
    uint16_t wLength;
} ctrl_request;

uint8_t ep1_buf[1];

extern uint32_t _sidata, _sdata, _edata, _ebss, _sbss;

void flash_to_ram(){

    uint32_t *p_idata = &_sidata;

    for(uint32_t *p_data = &_sdata; p_data < &_edata; *p_data++ = *p_idata++);
    for(uint32_t *p_bss = &_sbss; p_bss < &_ebss; *p_bss++ = 0);

}

void clocks_init(){

    REG(RCC + CFGR0) = PLLSRC | SW_PLL | PLL_MUL6;
    REG(RCC + CTLR) = PLLON | HSEON;
    while(!(REG(RCC + CTLR) & PLLRDY));
}

void usb_init(){

    REG(RCC + APB1PCENR) = USBEN;

    REG(USB_PMA + 0*16 + ADDR_TX) = 12;
    REG(USB_PMA + 0*16 + ADDR_RX) = 76;

    REG(USB_PMA + 1*16 + ADDR_TX) = 84;

    REG(USB + CNTR) = CTRM | RESETM;

    REG(EXTEN_CTR) = USBDPU;

}

void handle_reset(){

    REG(USB + ISTR) = 0;
    REG(USB + EPR0) = RX_VALID | CONTROL | TX_NAK | 0;
    REG(USB + DADDR) = EF;
    REG(USB + EPR1) = RX_DISABLED | INTERRUPT | TX_NAK | 1;
}

void pma_to_buffer(uint8_t ep, uint8_t* buf){

    uint8_t len = REG(USB_PMA + ep*16 + COUNT_RX) & COUNT_BITS;
    uint32_t* pma = (uint32_t *)(USB_PMA + 2*REG(USB_PMA + ep*16 + ADDR_RX));
    for(uint8_t i = 0; 2*i < len; i++)
        ((uint16_t*)buf)[i] = pma[i];
}

void buffer_to_pma(uint8_t ep, uint8_t* buf, uint8_t len){

    uint32_t* pma = (uint32_t *)(USB_PMA + 2*REG(USB_PMA + ep*16 + ADDR_TX));
    for (uint8_t i = 0; 2*i < len; i++)
        pma[i] = ((uint16_t *)buf)[i];
    REG(USB_PMA + ep*16 + COUNT_TX) = len;
}

void handle_get_request(){

    switch(ctrl_request.wValue_high){
    case 1:
        buffer_to_pma(0, deviceDescriptor, ctrl_request.wLength);
        break;
    case 2:
        buffer_to_pma(0, configDescriptor, ctrl_request.wLength);
        break;
    case 3:
        switch(ctrl_request.wValue_low){
            case 0:
                buffer_to_pma(0, langidDescriptor, sizeof langidDescriptor);
                break;
            case 1:
                buffer_to_pma(0, manufacturerDescriptor, sizeof manufacturerDescriptor);
                break;
            case 2:
                buffer_to_pma(0, productDescriptor, sizeof productDescriptor);
                break;
            case 3:
                buffer_to_pma(0, serialDescriptor, sizeof serialDescriptor);
        }
    }
}

void handle_set_request(){

        REG(USB_PMA + 0*16 + COUNT_TX) = 0;
}

void handle_hid_get_request(){

    buffer_to_pma(0, reportDescriptor, ctrl_request.wLength);
}

void handle_ep0_setup(){

    pma_to_buffer(0, (uint8_t *)&ctrl_request);
    switch(ctrl_request.bmRequestType){
    case 0:
        handle_set_request();
        break;
    case 0x21:
        handle_set_request();
        break;
    case 0x80:
        handle_get_request();
        break;
    case 0x81:
        handle_hid_get_request();
    }

    REG(USB + EPR0) = CONTROL | CTR_TX | ((TX_NAK) ^ (TX_VALID));
}

void handle_ep0_in(){

    if(ctrl_request.bRequest == 5)
        REG(USB + DADDR) = EF | ctrl_request.wValue_low;

    REG(USB + EPR0) = CTR_RX | ((RX_NAK) ^ (RX_VALID)) | CONTROL;
}

void handle_ep0_out(){

    REG(USB + EPR0) = ((RX_NAK) ^ (RX_VALID)) | CONTROL | CTR_TX;
}

void handle_ep0(void){

    switch(REG(USB + EPR0) & (CTR_RX | SETUP | CTR_TX)){
    case CTR_RX | SETUP:
        handle_ep0_setup();
        break;
    case CTR_TX:
        handle_ep0_in();
        break;
    case CTR_RX:
        handle_ep0_out();
    }
}

void handle_ep1(){

    REG(USB + EPR1) = 1;
}

void handle_ctr(uint8_t ep){

    if(ep == 0)
        handle_ep0();
    else
        handle_ep1();
}

void usb_send(uint8_t len){

    buffer_to_pma(1, ep1_buf, len);
    REG(USB + EPR1) = CTR_TX | INTERRUPT | ((TX_NAK) ^ (TX_VALID)) | 1;
}

void USB_LP_CAN1_RX0_IRQ_Handler() __attribute__((interrupt));

void USB_LP_CAN1_RX0_IRQ_Handler(){

    uint16_t istr = REG(USB + ISTR);
    if(istr & RESET)
        handle_reset();
    else
        handle_ctr(istr & 0xf);
}

void delay(uint32_t t){

    for(uint32_t i = t; i; i--)
        __asm__ volatile("nop");
}

void prog(){

    flash_to_ram();
    clocks_init();
    usb_init();

    REG(PFIC + IENR1) = 1<<(USB_LP_CAN1_RX0_IRQn - 32);

    while(1){
        ep1_buf[0] = 0x5;
        usb_send(1);
        delay(100000);
        ep1_buf[0] = 0;
        usb_send(1);
        delay(1000000);
    }
}
