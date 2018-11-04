#include "flash.h"
#include <stm32f10x.h>
#include <stm32f10x_flash.h>


#define FALSE								0
#define TRUE								1

static u32 ApplicationAddress = 0;
static u32 ApplicationSize		= 0;

#define PAGE_SIZE                         (0x400)    /* 1 Kbyte */
#define FLASH_SIZE                        (0x20000)  /* 128 KBytes */


/*******************************************************************************
* Function Name :unsigned int FLASH_PagesMask(volatile unsigned int Size)
* Description   :?????? ???
* Input         :
* Output        :
* Other         :
* Date          :2013.02.24
*******************************************************************************/
static u32 FLASH_PagesMask(vu32 Size)
{
    u32 pagenumber = 0x0;
    u32 size = Size;

    if ((size % PAGE_SIZE) != 0)
    {
        pagenumber = (size / PAGE_SIZE) + 1;
    }
    else
    {
        pagenumber = size / PAGE_SIZE;
    }
    return pagenumber;
}


/*******************************************************************************
* Function Name :u32 FLASH_WriteBank(u32 *pData, u32 addr, u32 size)
* Description   :??????
* Input         :pData:??;addr:?????;size:??
* Output        :TRUE:??,FALSE:???
* Other         :
* Date          :2013.02.24
*******************************************************************************/
u32 FLASH_WriteBank(u8 *pData, u32 addr, u16 size)
{
    vu16 *pDataTemp = (vu16 *)pData;
    vu32 temp = addr;
//    FLASH_Status FLASHStatus = FLASH_COMPLETE;
//    u32 NbrOfPage = 0;
//
//    NbrOfPage = FLASH_PagesMask(addr + size - ApplicationAddress);
//    for (; (m_EraseCounter < NbrOfPage) && (FLASHStatus == FLASH_COMPLETE); m_EraseCounter++)
//    {
//        FLASHStatus = FLASH_ErasePage(ApplicationAddress + (PAGE_SIZE * m_EraseCounter));
//    }

    for (; temp < (addr + size); pDataTemp++, temp += 2)
    {
				//?????
        FLASH_ProgramHalfWord(temp, *pDataTemp);
        if (*pDataTemp != *(vu16 *)temp)
        {
            return FALSE;
        }
    }

    return TRUE;
}


static u8 buffer[2];
static u8 buffer_index = 0;
static u32 buffer_addr = 0;
u32 FLASH_AppendOneByte(u8 Data)
{
	buffer[buffer_index++] = Data;
	if (buffer_index >= sizeof(buffer))
	{
		FLASH_WriteBank(buffer,buffer_addr,sizeof(buffer));
		buffer_addr += sizeof(buffer);
		buffer_index = 0;
	}
	//
}

u32 FLASH_AppendBuffer(u8 *Data , u32 size)
{
	u32 i=0;
	for(i=0;i<size;i++)
	{
		FLASH_AppendOneByte(Data[i]);
	}
	
	//
}

void FLASH_AppendEnd(void)
{
	if (buffer_index > 0)
	{
		FLASH_WriteBank(buffer,buffer_addr,sizeof(buffer));
		buffer_addr += sizeof(buffer);
	}
	//
}
/*******************************************************************************
* Function Name :void FLASH_ProgramDone(void)
* Description   :????
* Input         :
* Output        :
* Other         :
* Date          :2013.02.24
*******************************************************************************/
void FLASH_ProgramStart(u32 addr , u32 size)
{
    FLASH_Status FLASHStatus = FLASH_COMPLETE;
    u32 NbrOfPage = 0;
    vu32 EraseCounter = 0;

    FLASH_Unlock();
//    m_EraseCounter = 0;
	
		ApplicationAddress = addr;
		ApplicationSize = size;
	
		buffer_index = 0;
		buffer_addr = ApplicationAddress;

    NbrOfPage = FLASH_PagesMask(ApplicationSize);
    for (; (EraseCounter < NbrOfPage) && (FLASHStatus == FLASH_COMPLETE); EraseCounter++)
    {
        FLASHStatus = FLASH_ErasePage(ApplicationAddress + (PAGE_SIZE * EraseCounter));
    }
}


/*******************************************************************************
* Function Name :void FLASH_ProgramDone(void)
* Description   :????
* Input         :
* Output        :
* Other         :
* Date          :2013.02.24
*******************************************************************************/
void FLASH_ProgramDone(void)
{
    FLASH_Lock();
}
