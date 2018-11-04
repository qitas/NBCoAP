#include <stm32f10x.h>
#include <stdio.h>

#define IRL	UARTdelayUS(1);
#define IRH	UARTdelayUS(1);

#define BAUD_115200 1000

void UARTdelayUS(int delay){};

void IO_TXD(u8 Data)
{
	u8 i = 0;
	
	u8 Data2;
	u8 jiou = 0;
	Data2 = Data;
	for(i = 0; i < 8; i++)
	{
		if(Data2&0x01)
		{
		}			
		else
		{
			jiou ++;
		}
		
		Data2 = Data2>>1;
	}
	 
	
	//OI_TXD = 0;
	IRL;
	UARTdelayUS(BAUD_115200);
	for(i = 0; i < 8; i++)
	{
		if(Data&0x01)
		{
			IRH;
		}			
		else
		{
			IRL;
		}			
		UARTdelayUS(BAUD_115200);
		Data = Data>>1;
	}
	
//	if ((jiou % 2) == 0)
//	{
//		IRL;
//	}else{
//		IRH;
//	}
	
	UARTdelayUS(BAUD_115200);
	
	IRH;
	UARTdelayUS(BAUD_115200);

	
		
}