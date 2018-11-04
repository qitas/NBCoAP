#ifndef __standby_h__
#define __standby_h__

enum {
	STANDBY_STATUS_INIT = 0,
	STANDBY_STATUS_PAIZHAO_1,
	STANDBY_STATUS_PAIZHAO_2,
	STANDBY_STATUS_PAIZHAO_3,
	STANDBY_STATUS_PAIZHAO_FINISH,
	STANDBY_STATUS_UPLOAD,
	STANDBY_STATUS_UPLOAD_FINISH,
	STANDBY_STATUS_ALARM,
};

void Sys_Standby(void);
void Sys_Enter_Standby(void);
void WKUP_Pin_Init(void);

#define READ_TEST_PIN GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_0)

#endif
