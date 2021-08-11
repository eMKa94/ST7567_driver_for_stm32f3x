/*
    board_rs485.c
    IPA board rs485 Interface
*/

#include "board_rs485.h"

LL_USART_InitTypeDef        boardRs485Usart;
LL_USART_ClockInitTypeDef   boardRs485UsartClock;

#define boardRs485ClockEnable() LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_USART3)

#define boardRs485UsartPort_def USART3

#define boardRs485TxPort_def    GPIOC
#define boardRs485TxPin_def     LL_GPIO_PIN_10
ErrorStatus boardRs485TxInit(void);

#define boardRs485RxPort_def    GPIOC
#define boardRs485RxPin_def     LL_GPIO_PIN_11
ErrorStatus boardRs485RxInit(void);

#define boardRs485DirPort_def    GPIOC
#define boardRs485DirPin_def     LL_GPIO_PIN_12
ErrorStatus boardRs485DirInit(void);
#define boardRs485SetDirTx()       LL_GPIO_SetOutputPin(boardRs485DirPort_def, boardRs485DirPin_def);
#define boardRs485SetDirRx()       LL_GPIO_ResetOutputPin(boardRs485DirPort_def, boardRs485DirPin_def);

void boardRs485Init(void)
{

    LL_RCC_SetUSARTClockSource(LL_RCC_USART1_CLKSOURCE_HSI);

    boardRs485ClockEnable();
    boardRs485Disable();

    (void)boardRs485TxInit();
    (void)boardRs485RxInit();
    (void)boardRs485DirInit();
     
    LL_USART_StructInit(&boardRs485Usart);
    LL_USART_Init(boardRs485UsartPort_def, &boardRs485Usart);

    LL_USART_ClockStructInit(&boardRs485UsartClock);
    LL_USART_ClockInit(boardRs485UsartPort_def, &boardRs485UsartClock);
}

void boardRs485Reset(void)
{
    boardRs485Disable();
    LL_USART_Init(boardRs485UsartPort_def, &boardRs485Usart);
    boardRs485Enable();
}

ErrorStatus boardRs485TxInit(void)
{

    LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOC);

    
    LL_GPIO_InitTypeDef     boardRs485Tx;

    boardRs485Tx.Pin        = boardRs485TxPin_def;
    boardRs485Tx.Mode       = LL_GPIO_MODE_ALTERNATE;
    boardRs485Tx.Alternate  = LL_GPIO_AF_7;

    return(LL_GPIO_Init(boardRs485TxPort_def, &boardRs485Tx));

}

ErrorStatus boardRs485RxInit(void)
{
    LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOC);

    LL_GPIO_InitTypeDef     boardRs485Rx;

    boardRs485Rx.Pin        = boardRs485RxPin_def;
    boardRs485Rx.Mode       = LL_GPIO_MODE_ALTERNATE;
    boardRs485Rx.Alternate  = LL_GPIO_AF_7;

    return(LL_GPIO_Init(boardRs485RxPort_def, &boardRs485Rx));
}

ErrorStatus boardRs485DirInit(void)
{
    LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOC);

    LL_GPIO_InitTypeDef     boardRs485Dir;

    boardRs485Dir.Pin        = boardRs485DirPin_def;
    boardRs485Dir.Mode       = LL_GPIO_MODE_OUTPUT;
    boardRs485Dir.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
    return(LL_GPIO_Init(boardRs485RxPort_def, &boardRs485Dir));
}

void boardRs485Disable(void)
{
    LL_USART_Disable(boardRs485UsartPort_def);
}

void boardRs485Enable(void)
{
    LL_USART_Enable(boardRs485UsartPort_def);
}

inline void boardRs485SetDirectionTransmit(void)
{
    boardRs485SetDirTx();
}

inline void boardRs485SetDirectionReceive(void)
{
    boardRs485SetDirRx();
}

void boardRs485SetBaudrate(rs485Baudrate_t baudrate)
{
    switch (baudrate)
    {
        case baudrate_4800:
            boardRs485Usart.BaudRate = 4800;
            break;
        case baudrate_9600:
            boardRs485Usart.BaudRate = 9600;
            break;
        case baudrate_19200:
            boardRs485Usart.BaudRate = 19200;
            break;
        case baudrate_38400:
            boardRs485Usart.BaudRate = 38400;
            break;
        case baudrate_57600:
            boardRs485Usart.BaudRate = 57600;
            break;
        case baudrate_115200:
            boardRs485Usart.BaudRate = 115200;
            break;
        default:
            break;
    }
    boardRs485Reset();
}

void boardRs485SetParity(rs485Parity_t parity)
{
    switch (parity)
    {
    case parity_none:
        boardRs485Usart.Parity = LL_USART_PARITY_NONE;
        break;
    case parity_even:
        boardRs485Usart.Parity = LL_USART_PARITY_EVEN;
        break;
    case parity_odd:
        boardRs485Usart.Parity = LL_USART_PARITY_ODD;
    default:
        break;
    }
    boardRs485Reset();
}

void boardRs485SetStopBits(rs485StopBits_t stopBits)
{
    switch (stopBits)
    {
        case stopBits_1:
            boardRs485Usart.StopBits = LL_USART_STOPBITS_1;
            break;
        case stopBits_2:
            boardRs485Usart.StopBits = LL_USART_STOPBITS_2;
            break;
        
        default:
            break;
    }
    boardRs485Reset();
}

inline void boardRs485SendChar(char charToTransmit)
{
    LL_USART_TransmitData8(boardRs485UsartPort_def, charToTransmit);
}

inline char boardRs485GetChar(void)
{
    return((char)LL_USART_ReceiveData8(boardRs485UsartPort_def));
}
