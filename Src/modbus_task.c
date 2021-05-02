#include "stm32f7xx_hal.h"
#include "cmsis_os.h"

#include "mb.h"
#include "mbport.h"

#define REG_INPUT_START 1000
#define REG_INPUT_NREGS 8
#define REG_HOLDING_START 1
#define REG_HOLDING_NREGS 32

static USHORT usRegInputStart = REG_INPUT_START;
static USHORT usRegInputBuf[REG_INPUT_NREGS];
static USHORT usRegHoldingStart = REG_HOLDING_START;
static USHORT usRegHoldingBuf[REG_HOLDING_NREGS];

void ModbusRTUTask(void const *argument)
{
    /* ABCDEF */
    usRegInputBuf[0] = 11;
    usRegInputBuf[1] = 22;
    usRegInputBuf[2] = 33;
    usRegInputBuf[3] = 44;
    usRegInputBuf[4] = 55;
    usRegInputBuf[5] = 66;
    usRegInputBuf[6] = 77;
    usRegInputBuf[7] = 88;

    eMBErrorCode eStatus = eMBInit(MB_RTU, 1, 3, 19200, MB_PAR_EVEN);
    eStatus = eMBEnable();

    while (1)
    {
        eMBPoll();
    }
}

USHORT* getHoldingRegisters() {
    return usRegHoldingBuf;
}

eMBErrorCode
eMBRegInputCB(UCHAR *pucRegBuffer, USHORT usAddress, USHORT usNRegs)
{
    eMBErrorCode eStatus = MB_ENOERR;
    int iRegIndex;

    if ((usAddress >= REG_INPUT_START) && (usAddress + usNRegs <= REG_INPUT_START + REG_INPUT_NREGS))
    {
        iRegIndex = (int)(usAddress - usRegInputStart);
        while (usNRegs > 0)
        {
            *pucRegBuffer++ =
                (unsigned char)(usRegInputBuf[iRegIndex] >> 8);
            *pucRegBuffer++ =
                (unsigned char)(usRegInputBuf[iRegIndex] & 0xFF);
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

eMBErrorCode
eMBRegHoldingCB(UCHAR *pucRegBuffer, USHORT usAddress, USHORT usNRegs,
                eMBRegisterMode eMode)
{
    eMBErrorCode eStatus = MB_ENOERR;
    int iRegIndex;

    if ((usAddress >= REG_HOLDING_START) && (usAddress + usNRegs <= REG_HOLDING_START + REG_HOLDING_NREGS))
    {
        iRegIndex = (int)(usAddress - usRegHoldingStart);
        switch (eMode)
        {
        case MB_REG_READ:
            while (usNRegs > 0)
            {
                *pucRegBuffer++ = (unsigned char)(usRegHoldingBuf[iRegIndex] >> 8);
                *pucRegBuffer++ = (unsigned char)(usRegHoldingBuf[iRegIndex] & 0xFF);
                iRegIndex++;
                usNRegs--;
            }
            break;

        case MB_REG_WRITE:
            while (usNRegs > 0)
            {
                usRegHoldingBuf[iRegIndex] = *pucRegBuffer++ << 8;
                usRegHoldingBuf[iRegIndex] |= *pucRegBuffer++;
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

eMBErrorCode
eMBRegCoilsCB(UCHAR *pucRegBuffer, USHORT usAddress, USHORT usNCoils,
              eMBRegisterMode eMode)
{
    return MB_ENOREG;
}

eMBErrorCode
eMBRegDiscreteCB(UCHAR *pucRegBuffer, USHORT usAddress, USHORT usNDiscrete)
{
    return MB_ENOREG;
}
