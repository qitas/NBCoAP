#ifndef __cbase64_h__
#define __cbase64_h__


struct dc_stream {
	char a,b,c,d;
	char status;
	unsigned int count;
};
extern int encode_base64(unsigned char * input , int inlen , char * output);
extern int decode_base64(const char * input , char * output , int * outlen);

void start_decode_base64_stream(struct dc_stream *ds);
int decode_base64_stream(const char input , unsigned char *out,struct dc_stream *ds);


#endif
