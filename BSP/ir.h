#ifndef __IR__
#define __IR__


void init_ir_gpio(void);
void openIR(void);
void closeIR(void);
void __send_ir_wakeup_cmd(void);
void PWMHONGWAI(void);

#endif