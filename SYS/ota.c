#include "ota.h"
#include "flash.h"
#include <stm32f0xx.h>


typedef  int (*pFunction)(void);
int JumpToApp(u32 appAddr)
{
	int ret = 0;
	pFunction JumpToApplication;
	u32 JumpAddress;
	
	//appAddr = CPU2_FW_ADDRESS;
	
	JumpAddress = *(u32*) (appAddr + 4);
	JumpToApplication = (pFunction)JumpAddress;
  /* Initialize user application's Stack Pointer */
  //__set_MSP(*(vu32*) appAddr);
  ret = JumpToApplication();
	
	return ret;
}

#ifdef USE_OTA


/**********************************************/
/* 函数功能；主函数                           */
/* 入口参数：无                               */
/**********************************************/
//#define APPLICATION_ADDRESS     ((uint32_t)0x08003000)
///* Relocate by software the vector table to the internal SRAM at 0x20000000 ***/  
//#if   (defined ( __CC_ARM ))
//  __IO uint32_t VectorTable[48] __attribute__((at(0x20000000)));
//#elif (defined (__ICCARM__))
//#pragma location = 0x20000000
//  __no_init __IO uint32_t VectorTable[48];
//#elif defined   (  __GNUC__  )
//  __IO uint32_t VectorTable[48] __attribute__((section(".RAMVectorTable")));
//#elif defined ( __TASKING__ )
//  __IO uint32_t VectorTable[48] __at(0x20000000);
//#endif
void VectorTable_Set(void)
{
  uint32_t i = 0;


  /* Copy the vector table from the Flash (mapped at the base of the application
     load address 0x08003000) to the base address of the SRAM at 0x20000000. */       
  for(i = 0; i < 48; i++)
  {
    *((uint32_t*)(0x20000000 + (i << 2)))=*(__IO uint32_t*)(APPLICATION_ADDRESS + (i<<2));
	}
	
  /* Enable the SYSCFG peripheral clock*/ 
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE); 
	//RCC_APB2PeriphResetCmd(RCC_APB2Periph_SYSCFG, ENABLE);
	/* Remap SRAM at 0x00000000 */
  SYSCFG_MemoryRemapConfig(SYSCFG_MemoryRemap_SRAM);
	
}	

#endif