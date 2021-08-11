/*
    board_rs485.h
    IPA board rs485 Interface
*/

#ifndef BOARD_RS485_h
#define BOARD_RS485_h

#include "stm32f3xx_ll_usart.h"
#include "stm32f3xx_ll_gpio.h"
#include "stm32f3xx_ll_rcc.h"
#include "stm32f3xx_ll_bus.h"

extern LL_USART_InitTypeDef        boardRs485Usart;

void boardRs485Init(void);
void boardRs485Reset(void);

void boardRs485Enable(void);
void boardRs485Disable(void);

void boardRs485SetDirectionTransmit(void);
void boardRs485SetDirectionReceive(void);

typedef enum rs485Baudrate_t
{
    baudrate_4800,
    baudrate_9600,
    baudrate_19200,
    baudrate_38400,
    baudrate_57600,
    baudrate_115200
}rs485Baudrate_t;
void boardRs485SetBaudrate(rs485Baudrate_t baudrate);

typedef enum rs485Parity_t
{
    parity_none,
    parity_even,
    parity_odd
}rs485Parity_t;
void boardRs485SetParity(rs485Parity_t parity);

typedef enum rs485StopBits_t
{
    stopBits_1,
    stopBits_2
}rs485StopBits_t;

void boardRs485SetStopBits(rs485StopBits_t stopBits);
void boardRs485SendChar(char charToTransmit);
char boardRs485GetChar(void);

#endif
