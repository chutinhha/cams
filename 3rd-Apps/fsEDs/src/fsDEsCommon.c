/***************************************************************************
 ** File            :   fsDEsCommon.c
 ** 
 ** Description     :   common function used through out the program
 **
 ** History
 **     Created     :   Nov 5  2012
 **     Modified    :	Nov 14 2012
 **
 ** Notes           :
 **
 ** ToDo            :
 **
 ***************************************************************************/

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

#include "fsDEsCommon.h"
#include "fsDEsHash.h"
#include "fsDEsRandom.h"

void block_8_to_32
 (uint8_t *bl8, uint32_t *bl32, size_t size)
 {
	size_t i;
	
	for(i=0; i<size; i+=4)
		bl32[i/4]=(bl8[i+0]<<24)+(bl8[i+1]<<16)+(bl8[i+2]<<8)+bl8[i+3];
 }

void block_32_to_8
 (uint8_t *bl8, uint32_t *bl32, size_t size)
 {
	size_t i;
	
	for(i=0; i<size; i+=4){
		bl8[i+0]=(bl32[i/4]&0xff000000)>>24;
		bl8[i+1]=(bl32[i/4]&0x00ff0000)>>16;
		bl8[i+2]=(bl32[i/4]&0x0000ff00)>>8;
		bl8[i+3]=(bl32[i/4]&0x000000ff);
	}
 }
 
size_t fsize
 (FILE *in)
 {
	size_t fs, offset;
	offset=ftell(in);
 	fseek(in, 0, SEEK_END);
	fs=ftell(in);
	fseek(in, offset, SEEK_SET);
	return fs;
 }

uint8_t * fs_read_key_file
 (uint8_t *key_file_name)
 {
	FILE *key_file;
	uint8_t *buf;
    
	key_file = fopen(key_file_name, "rb");
	if (key_file == NULL){
		perror(key_file_name);
		exit(1);
	}
	if (fsize(key_file)<65536){
		printf("key_file_size < 65536\n");
		exit(1);
	}
	
	buf = (uint8_t *) malloc (65536);
	if (buf == NULL){
		perror("fs_read_key_file buf_malloc_fail:");
		exit(1);
	}
	
	fread(buf,1,65536,key_file);
	
	fclose(key_file);
	return buf;
}

//-----------------------------------------------------------------------------------
// Function     :   fs_gen_key_file
// Description  :   generate the fsDEs_s key file to use in encryption/decryption
// Arguments    :   @ key_file_name  -   name of the key file we are going to use
//              :   @ seed           -  future seed parameter value, we will skip that
// return       :   none
// notes        :   
//-----------------------------------------------------------------------------------
void fs_gen_key_file
 (char *key_file_name, uint32_t seed)
 {
	FILE *key_file;
	size_t i;
	uint8_t *buf;
	
	fs_srandom(seed);
	
    // key file has been created, open it in write mode, more more details check man page for open
	key_file = fopen(key_file_name, "wb");
	if (key_file == NULL){
		perror(key_file_name);
		exit(1);
	}

	buf = (uint8_t *) malloc (65536);
	if (buf == NULL){
		perror("fs_read_key_file buf_malloc_fail:");
		exit(1);
	}
	for(i=0; i<65536;i++)
		buf[i]=fs_random_32()%256;

	fwrite(buf,1,65536,key_file);

	fclose(key_file);
	free(buf);
}

void fs_cipher_calc_key_pair
 (char *password, uint32_t *key, uint32_t *sub_key)
 {
	struct fs_hash_context fsDEsH;
	uint32_t *data, i, len;
	
	for(i=0; password[i]!='\0'; i++); //strlen analog
	len=i;
	
	data = (uint32_t *) malloc (sizeof(uint32_t) * len);
	if(data == NULL){
		perror("fs_cipher_calc_key_pair data_malloc_fail:");
		exit(1);
	}
	
	for(i=0; i<len; i++)
		data[i]=(uint32_t)password[i];
	
	fs_hash_init(&fsDEsH);
	fs_hash_calc(data, len, &fsDEsH);

	*key=fsDEsH.hash[0];
	*sub_key=fsDEsH.hash[1];
	
	free(data);
 }

uint8_t * fs_cipher_token
 (uint32_t *token)
 {
	uint32_t tmp=0, i, n=0;
	uint8_t *buf;

	struct fs_hash_context fsDEsH;
    fs_hash_init(&fsDEsH);
	//uint32_t tmp=0, i, n=0;
	//uint8_t *buf;
	
	buf = (uint8_t *) malloc (65536);
	if(buf == NULL){
		perror("fs_cipher_token buf_malloc_fail:");
		exit(1);
	}
	
	for(i=0;i<8192;i++){
        fs_hash_step(token[0], &fsDEsH);
		fs_hash_step(token[1], &fsDEsH);
		fs_hash_step(token[2], &fsDEsH);
		fs_hash_step(token[3], &fsDEsH);
        fs_hash_step(tmp, &fsDEsH);
        
        tmp=fsDEsH.hash[1];
        buf[n+0]=(fsDEsH.hash[0]>>0)&0xff;
		buf[n+1]=(fsDEsH.hash[0]>>8)&0xff;
		buf[n+2]=(fsDEsH.hash[0]>>16)&0xff;
		buf[n+3]=(fsDEsH.hash[0]>>24)&0xff;
		
        buf[n+4]=(fsDEsH.hash[1]>>0)&0xff;
        buf[n+5]=(fsDEsH.hash[1]>>8)&0xff;
        buf[n+6]=(fsDEsH.hash[1]>>16)&0xff;
        buf[n+7]=(fsDEsH.hash[1]>>24)&0xff;
		n+=8;
    }
	return buf;
}
