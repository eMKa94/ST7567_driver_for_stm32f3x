/*
 * modbus_app.c
 *
 *  Created on: 16 gru 2020
 *      Author: mkowalski
 */

#include "modbus.h"
#include "board.h"


/* ----------------------- Static variables ---------------------------------*/
USHORT   usRegInputStart = REG_INPUT_START;
USHORT   usRegInputBuf[REG_INPUT_NREGS];

USHORT	usRegHoldingStart = REG_HOLDING_START;
USHORT	usRegHoldingBuf[REG_HOLDING_NREGS];

void MB_init(void)
{
	uint32_t	modbusBaudrate;
	eMBParity	modbusParity;
	rx_enable_request = 0;
	
    switch (rs485Baudrate)
    {
        case baudrate_4800:
            modbusBaudrate = 4800;
            break;
        
        case baudrate_9600:
            modbusBaudrate = 9600;
            break;
        
        case baudrate_19200:
            modbusBaudrate = 19200;
            break;
        
        case baudrate_38400:
            modbusBaudrate = 38400;
            break;
        
        case baudrate_57600:
            modbusBaudrate = 57600;
            break;
        
        case baudrate_115200:
            modbusBaudrate = 115200;
            break;

        default:
			modbusBaudrate = 9600;
            break;

    }

	switch (rs485Parity)
	{
		case parity_even:
			modbusParity = MB_PAR_EVEN;
			break;
		case parity_odd:
			modbusParity = MB_PAR_ODD;
			break;
		case parity_none:
			modbusParity = MB_PAR_NONE;
			break;
		default:
			modbusParity = MB_PAR_NONE;
			break;
	}
	eMBDisable();
	eMBInit( MB_RTU, (int)modbusId, 0, modbusBaudrate, modbusParity );
	eMBEnable(  );	//Enable the Modbus Protocol Stack.
}

eMBErrorCode	eMBRegInputCB( UCHAR * pucRegBuffer, USHORT usAddress, USHORT usNRegs )
{
    eMBErrorCode    eStatus = MB_ENOERR; 
    int             iRegIndex;

    if( ( usAddress >= REG_INPUT_START )  &&	( usAddress + usNRegs <= REG_INPUT_START + REG_INPUT_NREGS ) )
    {
        iRegIndex = ( int )( usAddress - usRegInputStart );
        while( usNRegs > 0 )
        {

            *pucRegBuffer++ = ( unsigned char )( usRegInputBuf[iRegIndex] >> 8 );
            *pucRegBuffer++ = ( unsigned char )( usRegInputBuf[iRegIndex] & 0xFF );
            iRegIndex++;
            usNRegs--;

        }
    }
    else
    {
        eStatus = MB_ENOREG;
    }

    return eStatus;

}

eMBErrorCode	eMBRegHoldingCB( UCHAR * pucRegBuffer, USHORT usAddress, USHORT usNRegs, eMBRegisterMode eMode )
{

	eMBErrorCode    eStatus = MB_ENOERR;
	int				iRegIndex;

	if ( ( usAddress >= REG_HOLDING_START ) && ( usAddress + usNRegs <= REG_HOLDING_START + REG_HOLDING_NREGS ) )
	{

		iRegIndex = usAddress - usRegHoldingStart;

		switch (eMode)
		{
			case MB_REG_READ:
				while (usNRegs > 0)
				{

					*pucRegBuffer++ = ( unsigned char )( usRegHoldingBuf[iRegIndex] >> 8 );
		            *pucRegBuffer++ = ( unsigned char )( usRegHoldingBuf[iRegIndex] & 0xFF );

		            iRegIndex++;
		            usNRegs--;

				}
				break;

			case MB_REG_WRITE:
				while (usNRegs > 0)
				{

					USHORT holding_high	= *pucRegBuffer++;
					USHORT holding_low	= *pucRegBuffer++;

					usRegHoldingBuf[iRegIndex] = ( holding_high << 8 ) | (holding_low);

		            iRegIndex++;
		            usNRegs--;

				}
		}
	}
	else
	{
		eStatus = MB_ENOREG;
	}
		return eStatus;
}

eMBErrorCode	eMBRegCoilsCB( UCHAR * pucRegBuffer, USHORT usAddress, USHORT usNCoils, eMBRegisterMode eMode )
{

    return MB_ENOREG;

}

eMBErrorCode eMBRegDiscreteCB( UCHAR * pucRegBuffer, USHORT usAddress, USHORT usNDiscrete )
{

    return MB_ENOREG;

}

