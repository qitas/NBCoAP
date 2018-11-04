#include "cbase64.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
//#include "my_stdc_func/debugl.h"

//#include "osdef.h"
//#define RUN_TEST

#define malloc (void)malloc
#define free rt_free


#define np 0xFF

static const char alphabet64[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
static const char pad = '=';
static char table64vals[] =
{
	62, np, np, np, 63, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61, np, np, np, np, np,
	np, np,  0,  1,  2,  3,  4,  5,  6,  7,  8,  9, 10, 11, 12, 13, 14, 15, 16, 17,
	18, 19, 20, 21, 22, 23, 24, 25, np, np, np, np, np, np, 26, 27, 28, 29, 30, 31,
	32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51
};

static char table64( unsigned char c )
{
	return ( c < 43 || c > 122 ) ? np : table64vals[c-43];
}

int encode_base64(unsigned char * input , int inlen , char * output)
{
	int i = 0 , j = 0 ; char c;
	for(i=0;i<inlen;i++)
	{
		c = (input[i] >> 2) & 0x3f ;
		output[j++] = alphabet64[c];

		c = (input[i] << 4 ) & 0x3f ;

		if( ++i < inlen )
			c = ( c | ( ( input[i] >> 4 ) & 0x0f ));

		output[j++] = alphabet64[c];

		if( i < inlen )
		{
			c = ( ( input[i] << 2 ) & 0x3c );
			if( ++i < inlen )
				c = ( c | ( ( input[i] >> 6 ) & 0x03 ) );
			output[j++] = alphabet64[c];
		}
		else
		{
			++i;
			output[j++] = pad;
		}

		if( i < inlen )
		{
			c = ( input[i] & 0x3f );
			output[j++] = alphabet64[c];
		}
		else
		{
			output[j++] = pad;
		}

	}

	output[j] = 0x0; // 结尾必须是0X0保证是一个字符串.
	return 0;
	//
}



void start_decode_base64_stream(struct dc_stream *ds)
{
	memset(ds,0x0,sizeof(struct dc_stream));
	//
}

int decode_base64_stream(const char input , unsigned char *out,struct dc_stream *ds)
{
	int ret = -1;
	
	ds->count ++ ;
	
	
	//rt_kprintf("Count : %d / %d \r\n",ds->count,ds->status);
	switch(ds->status)
	{
		case 0:
		{
			ds->c = table64(input);
			ds->status = 1;
			break;
		}
		case 1:
		{
			ds->d = table64(input);
			ds->c = (char)( ( ds->c << 2 ) | ( ( ds->d >> 4 ) & 0x3 ) );
			
			
			*out = (unsigned char)ds->c;
			//xxxxxxxxxxxxx decode is c
			
			ds->status = 2;
			ret = 0;
			break;
		}
		case 2:
		{
			ds->c = input;
			if (pad == ds ->c)
			{
				ds->status = 0;
				break;
			}
			
			ds->c = table64(input);
			ds->d = (char)( ( ( ds->d << 4 ) & 0xf0 ) | ( ( ds->c >> 2 ) & 0xf ) );
			
			ds->status = 3;
			*out = (unsigned char)ds->d;
			ret = 0;
			//decode is d;
			
			break;
		}
		
		case 3:
		{
			ds->d = input;
			if( pad == ds->d )
			{
				ds->status = 0;
				break;
			}
			
			ds->d = table64(input);
			ds->c = (char)( ( ( ds->c << 6 ) & 0xc0 ) | ds->d );
			//decoded[j++] = ds->c;
			//decode is ccccccccc
			*out = (unsigned char)ds->c;
			ds->status = 0;
			ret = 0;
			break;
		}
		default:
			break;
	}
	
	
	return ret;
}

int decode_base64(const char * input , char * decoded, int * outlen)
{
	int i=0,j=0;
	int inlen = strlen(input);
	char c, d;
	for(i=0;i<inlen;++i)
	{
		c = table64(input[i]);
		++i;
		d = table64(input[i]);
		c = (char)( ( c << 2 ) | ( ( d >> 4 ) & 0x3 ) );
		decoded[j++] = c;
		
		
		
		if( ++i < inlen )
		{
			c = input[i];
			if( pad == c )
				break;

			c = table64(input[i]);
			d = (char)( ( ( d << 4 ) & 0xf0 ) | ( ( c >> 2 ) & 0xf ) );
			decoded[j++] = d;
		}
		
		

		if( ++i < inlen )
		{
			d = input[i];
			if( pad == d )
				break;

			d = table64(input[i]);
			c = (char)( ( ( c << 6 ) & 0xc0 ) | d );
			decoded[j++] = c;
		}

	}
	*outlen = j;
	decoded[j++] = 0x0;
	return 0;
}

void test_base64(void)
{
	//char input[] = "AHRlc3QxADExMTExMQ==";
	char input[64];
	int length = strlen(input);
	int i=0;
	unsigned char out;
	struct dc_stream obj;
	unsigned char input_hex[] = {0x01,0x02,0x03,0x04,0x05,0x06,0x01,0x02,0x03,0x04,0x05,0x06,0x01,0x02,0x03,0x04,0x05,0x06,0x01,0x02,0x03,0x04,0x05,0x06,0x01,0x02,0x03,0x04,0x05,0x06};
	
	encode_base64(input_hex,sizeof(input_hex),input);
	
	start_decode_base64_stream(&obj);
	for(i=0;i<strlen(input);i++)
	{
		if (decode_base64_stream(input[i],&out,&obj) == 0)
		{
			printf("[%02X]",out);
		}
	}
	
	printf("xxxxxxxxxxxxxx\r\n");
	
	//
}

#if 0
int main(int argc , char ** argv)
{
	char input[] = "AHRlc3QxADExMTExMQ==";
	char output[1024];
	memset(output,0x0,sizeof(output));
	int outputlen;
	decode_base64(input,output,&outputlen);
	printf("%s\n",output);
	return 0;
}

#endif

