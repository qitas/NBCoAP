#ifndef __config_h__
#define __config_h__

#include "config_def.h"




char * data_2_json(struct CONFIG_DATA * in_data);
void json_2_data(char * in_json , struct CONFIG_DATA * out_data);

void write_config(void);
struct CONFIG_DATA *read_config(void);



#endif
