#include "mem.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


struct MEM_DATA *mem = 0;



#define USE_MALLOC

#define MEMBLOCK_COUNT 4
#define MEMBLOCK_SIZE 1024



#ifdef USE_MALLOC
#else

struct MEMBLOCK{
	unsigned char data[MEMBLOCK_SIZE];
	unsigned char flag;

};



struct MEMBLOCK memblock[MEMBLOCK_COUNT];
#endif

#ifdef USE_MALLOC
#define MEM_DEBUG_INFO_COUNT 8
struct MEM_DEBUG_INFO {
	unsigned char flag;
	char filename[16];
	int line;
	unsigned char *p;
};

struct MEM_DEBUG_INFO mdi[MEM_DEBUG_INFO_COUNT];

void trace_mem(void)
{
	int i;
	printf("MEM TRACE START --- \r\n");
	for(i=0;i<MEM_DEBUG_INFO_COUNT;i++)
	{
		if (mdi[i].flag == 1)
		{
			printf("MEM TRACE %s %d %x \r\n",mdi[i].filename,mdi[i].line,mdi[i].p);
		}
		//
	}
	
}

#endif

void init_mem(void)
{
	#ifdef USE_MALLOC
	memset(mdi,0x0,sizeof(mdi));
	#else
	memset(memblock,0x0,sizeof(memblock));
	#endif
}

unsigned char* alloc_mem(char *file , int line , int size)
{
	int i=0;
	#ifdef USE_MALLOC
	unsigned char * p = (unsigned char*)malloc(size);
	
	printf("ALLOC [%x] [%s : %d]\r\n",(unsigned int)p,file,line);
	
	for(i=0;i<MEM_DEBUG_INFO_COUNT;i++)
	{
		if (mdi[i].flag == 0)
		{
			mdi[i].flag = 1;
			snprintf(mdi[i].filename,sizeof(mdi[i].filename),"%s",file);
			mdi[i].line = line;
			mdi[i].p = p;
			break;
		}
		//
	}
	
	if (p == 0)
	{
		//printf("ALLOC MEM ERR %s %d %d!!!\r\n",file,line,size);
		for(;;){printf("ALLOC MEM ERR %s %d %d!!!\r\n",file,line,size);trace_mem();};
	}
	return p;
	#else
	int i;
	for(i=0;i<MEMBLOCK_COUNT;i++)
	{
		if (memblock[i].flag == 0)
		{
			memblock[i].flag = 1;
			memset(memblock[i].data,0x0,MEMBLOCK_SIZE);
			return memblock[i].data;
		}
	}
	printf("!!!!!!!!!!!!!!!!!!!!!! alloc mem err \r\n");
	for(;;){}
	return 0;
	
	#endif
	//
}

void free_mem(char *file , int line , unsigned char *mem)
{
	#ifdef USE_MALLOC
	int i;
	
	printf("FREE [%x]\r\n",(unsigned int)mem);
	
	for(i=0;i<MEM_DEBUG_INFO_COUNT;i++)
	{
		if (mdi[i].flag == 1)
		{
			if (mdi[i].p == mem)
			{
				mdi[i].flag = 0;
				mdi[i].p = 0;
				free(mem);
				return;
			}
			
		}
		//
	}
	printf("double free! %s %d\r\n",file,line);
	for(;;){};
	
	#else
	int i;
	for(i=0;i<MEMBLOCK_COUNT;i++)
	{
		if (memblock[i].data == mem)
		{
			memblock[i].flag = 0;
		}
	}
	#endif
	//
}

