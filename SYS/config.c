#include "config.h"
#include "JSON/cjson.h"
#include "flash.h"
#include "common.h"
#include "mem.h"

char * data_2_json(struct CONFIG_DATA * in_data)
{
	char * p;
	cJSON * pJsonRoot = NULL;
	char tmpstr[32];
 
//user_app_conf
	
	DEBUG_ERROR(0);

	pJsonRoot = cJSON_CreateObject();
	if(NULL == pJsonRoot){return 0;}
	
	cJSON_AddNumberToObject(pJsonRoot, "version", mem->cfg_data.version);
	cJSON_AddStringToObject(pJsonRoot, "ipaddress", mem->cfg_data.ipaddress);
	cJSON_AddStringToObject(pJsonRoot, "ipport", mem->cfg_data.ipport);
	
	cJSON_AddStringToObject(pJsonRoot, "phonenum0", mem->cfg_data.phonenum[0]);
	cJSON_AddStringToObject(pJsonRoot, "phonenum1", mem->cfg_data.phonenum[1]);
	cJSON_AddStringToObject(pJsonRoot, "phonenum2", mem->cfg_data.phonenum[2]);
	
	cJSON_AddNumberToObject(pJsonRoot, "phonemode0", mem->cfg_data.phonemode[0]);
	cJSON_AddNumberToObject(pJsonRoot, "phonemode1", mem->cfg_data.phonemode[1]);
	cJSON_AddNumberToObject(pJsonRoot, "phonemode2", mem->cfg_data.phonemode[2]);
	
	cJSON_AddNumberToObject(pJsonRoot, "workmode", mem->cfg_data.workmode);
	
	cJSON_AddStringToObject(pJsonRoot, "JSON_FLAG", "JSON");
	
	DEBUG_ERROR(0);
	p = cJSON_Print(pJsonRoot);
	DEBUG_ERROR(0);
	// else use : 
	// char * p = cJSON_PrintUnformatted(pJsonRoot);
	if(NULL == p)
	{
			//convert json list to string faild, exit
			//because sub json pSubJson han been add to pJsonRoot, so just delete pJsonRoot, if you also delete pSubJson, it will coredump, and error is : double free
			DEBUG_ERROR(0);
			cJSON_Delete(pJsonRoot);
			DEBUG_ERROR(0);
			return 0;
	}
	//free(p);
	DEBUG_ERROR(0);
	cJSON_Delete(pJsonRoot);
	
	DEBUG_ERROR(0);
	
	return p;
	//
}
void json_2_data(char * in_json , struct CONFIG_DATA * out_data)
{
		cJSON * pSub;
		cJSON * pJson;
    if(NULL == in_json){return;}
    pJson = cJSON_Parse(in_json);
    if(NULL == pJson){return;}
		
		pSub = cJSON_GetObjectItem(pJson, "version");
    if(NULL != pSub)
    {mem->cfg_data.version = pSub->valueint;}
		
		pSub = cJSON_GetObjectItem(pJson, "ipaddress");
    if(NULL != pSub)
    {snprintf(mem->cfg_data.ipaddress,sizeof(mem->cfg_data.ipaddress),"%s",pSub->valuestring);}
		
		
		pSub = cJSON_GetObjectItem(pJson, "ipport");
    if(NULL != pSub)
    {snprintf(mem->cfg_data.ipaddress,sizeof(mem->cfg_data.ipport),"%s",pSub->valuestring);}
		
	
		pSub = cJSON_GetObjectItem(pJson, "phonenum0");
    if(NULL != pSub)
    {snprintf(mem->cfg_data.phonenum[0],sizeof(mem->cfg_data.phonenum[0]),"%s",pSub->valuestring);}
		
		pSub = cJSON_GetObjectItem(pJson, "phonenum1");
    if(NULL != pSub)
    {snprintf(mem->cfg_data.phonenum[1],sizeof(mem->cfg_data.phonenum[0]),"%s",pSub->valuestring);}
		
		pSub = cJSON_GetObjectItem(pJson, "phonenum2");
    if(NULL != pSub)
    {snprintf(mem->cfg_data.phonenum[2],sizeof(mem->cfg_data.phonenum[0]),"%s",pSub->valuestring);}
		
		pSub = cJSON_GetObjectItem(pJson, "phonemode0");
    if(NULL != pSub)
    {mem->cfg_data.phonemode[0] = pSub->valueint;}
		
		pSub = cJSON_GetObjectItem(pJson, "phonemode1");
    if(NULL != pSub)
    {mem->cfg_data.phonemode[1] = pSub->valueint;}
		
		pSub = cJSON_GetObjectItem(pJson, "phonemode2");
    if(NULL != pSub)
    {mem->cfg_data.phonemode[2] = pSub->valueint;}
		
		pSub = cJSON_GetObjectItem(pJson, "workmode");
    if(NULL != pSub)
    {mem->cfg_data.workmode = pSub->valueint;}
		
		cJSON_Delete(pJson);
		
		return ;
	//
}

void write_config(void)
{
	char *json;
	//json = (char*)alloc_mem(1024*2);
	json =data_2_json(&mem->cfg_data);
	
	if (json > 0)
	{
	
		printf("make json %d -- %s \r\n",strlen(json) , json);
		
		if (strlen(json) < (CONFIG_DATA_SIZE - CONFNIG_RAW_DATA_SIZE - 4))
		{
			FLASH_ProgramStart(CONFIG_DATA_ADDR,CONFIG_DATA_SIZE);
			FLASH_AppendBuffer(mem->cfg_rawdata,CONFNIG_RAW_DATA_SIZE);
			FLASH_AppendBuffer((unsigned char*)json,strlen(json));
			FLASH_AppendEnd();
			FLASH_ProgramDone();
			
		}else{
			
		}
		
		free(json); //json free;
		
		//free_mem(__FILE__,__LINE__,(unsigned char*)json);
	}
}
struct CONFIG_DATA *read_config(void)
{
	char *json;
	json = (char*)alloc_mem(__FILE__,__LINE__,CONFIG_DATA_SIZE - CONFNIG_RAW_DATA_SIZE);
	//json = (char*)CONFIG_DATA_ADDR;
	
	memcpy(mem->cfg_rawdata,(char*)CONFIG_DATA_ADDR,CONFNIG_RAW_DATA_SIZE);
	memcpy(json,(char*)CONFIG_DATA_ADDR + CONFNIG_RAW_DATA_SIZE , CONFIG_DATA_SIZE - CONFNIG_RAW_DATA_SIZE);
	json[CONFIG_DATA_SIZE - CONFNIG_RAW_DATA_SIZE - 1] = 0x0;
	
	//检查是不是合法的JSON字符串
	if (strstr(json,"JSON_FLAG"))
	{
		json_2_data(json,&mem->cfg_data);
	}else{
		
		//如果不是合法的字符串恢复默认
		memset(&mem->cfg_data,0x0,sizeof(struct CONFIG_DATA));
		sprintf(mem->cfg_data.ipaddress,"");
		sprintf(mem->cfg_data.ipport,"");
		
	}
	free_mem(__FILE__,__LINE__,(unsigned char*)json);
	
	return &mem->cfg_data;
}