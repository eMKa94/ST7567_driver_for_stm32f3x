/*
 * FreeModbus Libary: BARE Port
 * Copyright (C) 2006 Christian Walter <wolti@sil.at>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 *
 * File: $Id$
 */

/* ----------------------- Platform includes --------------------------------*/

#include "board_rs485.h"
#include "port.h"
/* ----------------------- Modbus includes ----------------------------------*/
#include "mb.h"
#include "mbport.h"


/* ----------------------- static functions ---------------------------------*/
//static void prvvUARTTxReadyISR( void );
//static void prvvUARTRxISR( void );

uint8_t tx_finish_counter = 0;

/* ----------------------- Start implementation -----------------------------*/
void
vMBPortSerialEnable( BOOL xRxEnable, BOOL xTxEnable )
{
	 /*
     * If xRXEnable enable serial receive interrupts. If xTxENable enable
     * transmitter empty interrupts.
     */

	if (xRxEnable)
	{
		LL_USART_EnableIT_RXNE(USART3);
		rx_enable_request = 1;
		rxOnTimeMs = board_runtime_ms+5;
	}
	else
	{
		LL_USART_DisableIT_RXNE(USART3);
	}

	if (xTxEnable)
	{
		LL_USART_EnableIT_TXE(USART3);
		boardRs485SetDirectionTransmit();
	}
	else
	{
		LL_USART_DisableIT_TXE(USART3);
	}	

}

BOOL xMBPortSerialInit( UCHAR ucPORT, ULONG ulBaudRate, UCHAR ucDataBits, eMBParity eParity )
{

	boardRs485Init();

	switch(ulBaudRate)
	{
    	case 4800:
			boardRs485SetBaudrate(baudrate_4800);
			break;
    	case 9600:
			boardRs485SetBaudrate(baudrate_9600);
			break;
    	case 19200:
			boardRs485SetBaudrate(baudrate_19200);
			break;
    	case 38400:
			boardRs485SetBaudrate(baudrate_38400);
			break;
    	case 57600:
			boardRs485SetBaudrate(baudrate_57600);
			break;
    	case 115200:
			boardRs485SetBaudrate(baudrate_115200);
			break;
		default:
			boardRs485SetBaudrate(baudrate_9600);
			break;
			
	}

	switch(eParity)
	{
		case MB_PAR_NONE:
			boardRs485SetParity(parity_none);
			break;
		case MB_PAR_EVEN:
			boardRs485SetParity(parity_even);
			break;
		case MB_PAR_ODD:
			boardRs485SetParity(parity_odd);
			break;
		default:
			break;
	}
	
	boardRs485SetDirectionReceive();
	NVIC_EnableIRQ(USART3_IRQn);
	NVIC_ClearPendingIRQ(USART3_IRQn);

	boardRs485Enable();
    return TRUE;
}

BOOL
xMBPortSerialPutByte( CHAR ucByte )
{

	/*
	 * Put a byte in the UARTs transmit buffer. This function is called
     * by the protocol stack if pxMBFrameCBTransmitterEmpty( ) has been
     * called.
     */
	//driver_enable();
	boardRs485SendChar(ucByte);
    return TRUE;

}

BOOL
xMBPortSerialGetByte( CHAR * pucByte )
{
    /* Return the byte in the UARTs receive buffer. This function is called
     * by the protocol stack after pxMBFrameCBByteReceived( ) has been called.
     */
	*pucByte = (char)boardRs485GetChar();
    return TRUE;
}

/* Create an interrupt handler for the transmit buffer empty interrupt
 * (or an equivalent) for your target processor. This function should then
 * call pxMBFrameCBTransmitterEmpty( ) which tells the protocol stack that
 * a new character can be sent. The protocol stack will then call 
 * xMBPortSerialPutByte( ) to send the character.
 */

/* Create an interrupt handler for the receive interrupt for your target
 * processor. This function should then call pxMBFrameCBByteReceived( ). The
 * protocol stack will then call xMBPortSerialGetByte( ) to retrieve the
 * character.
 */


//USART3 interrupt handler


void USART3_IRQHandler(void)
{
		//IDLE interrupt handler
	if (LL_USART_IsActiveFlag_IDLE(USART3) && LL_USART_IsEnabledIT_IDLE(USART3))
	{
		LL_USART_ClearFlag_IDLE(USART3);
	}
	//Transmission complete interrupt handler
	if (LL_USART_IsActiveFlag_TC(USART3) && LL_USART_IsEnabledIT_TC(USART3))
	{
		LL_USART_ClearFlag_TC(USART3);
	}

	//TX empty handler
	if (LL_USART_IsActiveFlag_TXE(USART3) && LL_USART_IsEnabledIT_TXE(USART3))
	{
		//driver_disable();
		pxMBFrameCBTransmitterEmpty(  );
	}

	//RX data received interrupt
	if (LL_USART_IsActiveFlag_RXNE(USART3) && LL_USART_IsEnabledIT_RXNE(USART3))
	{
		pxMBFrameCBByteReceived(  );
	}

}
