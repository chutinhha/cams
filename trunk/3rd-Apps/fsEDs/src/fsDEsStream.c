/***************************************************************************
 ** File            :   fsDEsStream.c
 ** 
 ** Description     :   create cipher toekn from the key file 
 **
 ** History
 **     Created     :   
 **     Modified    :
 **
 ** Notes           :
 **
 ** ToDo            :
 **
 ***************************************************************************/
 
#include <stdint.h>
#include <stdio.h>

#include "fsDEsCommon.h"
#include "fsDEsStream.h"

//-----------------------------------------------------------------------------------
// Function     :   fs_stream_modify
// Description  :   modifies the existing the cipher value
// Arguments    :   @cipher context structure
//              :   
// return       :   modified stream
// notes        :   
//-----------------------------------------------------------------------------------
uint32_t fs_stream_modify
 (uint32_t a, uint32_t b, struct fs_stream_context *fsDEs_s)
 {
	uint8_t k[4], m[4];
	
	k[0]=(a&0xff000000)>>24;
	k[1]=(a&0x00ff0000)>>16;
	k[2]=(a&0x0000ff00)>>8;
	k[3]=(a&0x000000ff);
	
	m[0]=(b&0xff000000)>>24;
	m[1]=(b&0x00ff0000)>>16;
	m[2]=(b&0x0000ff00)>>8;
	m[3]=(b&0x000000ff);
	
	k[0]=fsDEs_s->table[k[0]*256+m[0]];
	k[1]=fsDEs_s->table[k[1]*256+m[1]];
	k[2]=fsDEs_s->table[k[2]*256+m[2]];
	k[3]=fsDEs_s->table[k[3]*256+m[3]];
	
	return (k[3]<<24)+(k[0]<<16)+(k[1]<<8)+(k[2]);
}

//-----------------------------------------------------------------------------------
// Function     :   fs_stream_simple_init_key_file
// Description  :   initialize the key file and read the key pair value
// Arguments    :   @key file name
//              :   @stream context structure
// return       :   none
// notes        :   
//----------------------------------------------------------------------------------- 
void fs_stream_simple_init_key_file
 (uint8_t *password, uint8_t *key_file_name, struct fs_stream_context *fsDEs_s)
 {
	uint32_t key, sub_key;
	uint8_t *key_table;
	
	key_table=fs_read_key_file(key_file_name);
	fs_cipher_calc_key_pair(password, &key, &sub_key);
	fs_stream_init(key_table, key, sub_key, fsDEs_s);
}

//-----------------------------------------------------------------------------------
// Function     :   fs_stream_simple_init_token
// Description  :   initialize the token
// Arguments    :   @key file name
//              :   @stream context structure
// return       :   none
// notes        :   
//----------------------------------------------------------------------------------- 
void fs_stream_simple_init_token
 (uint8_t *password, uint32_t *token, struct fs_stream_context *fsDEs_s)
 {
	uint32_t key, sub_key;
	uint8_t *key_table;
	
	key_table=fs_cipher_token(token);
	fs_cipher_calc_key_pair(password, &key, &sub_key);
	fs_stream_init(key_table, key, sub_key, fsDEs_s);
}

//-----------------------------------------------------------------------------------
// Function     :   fs_stream_init
// Description  :   initialize stream with key and context structure
// Arguments    :   @table 
//              :   @stream context structure
// return       :   none
// notes        :   
//-----------------------------------------------------------------------------------
void fs_stream_init
 (uint8_t *key_table, uint32_t key, uint32_t sub_key, struct fs_stream_context *fsDEs_s)
 {
	fsDEs_s->table=key_table;
	fsDEs_s->sub_key=sub_key;
	fsDEs_s->key=fs_stream_modify(key, sub_key, fsDEs_s);
}

//-----------------------------------------------------------------------------------
// Function     :   fs_stream_next_key
// Description  :   gets the next key item, modifies the existing key item
// Arguments    :   @stream context structure
//              :   
// return       :   none
// notes        :   
//-----------------------------------------------------------------------------------
void fs_stream_next_key
 (struct fs_stream_context *fsDEs_s)
 {
	fsDEs_s->key=fs_stream_modify
		(fsDEs_s->key, (fsDEs_s->dec) & (fsDEs_s->sub_key), fsDEs_s);
}

//-----------------------------------------------------------------------------------
// Function     :   fs_stream_next_sub_key
// Description  :   optionally gets the next sub key item
// Arguments    :   @stream context structure
//              :   
// return       :   none
// notes        :   
//-----------------------------------------------------------------------------------
void fs_stream_next_sub_key (struct fs_stream_context *fsDEs_s)
 {
	fsDEs_s->sub_key=fs_stream_modify (fsDEs_s->sub_key, (fsDEs_s->key) | (fsDEs_s->sub_key), fsDEs_s);
}

//-----------------------------------------------------------------------------------
// Function     :   fs_stream_dec_step
// Description  :   starts decryption process by getting next stream item
// Arguments    :   @next	:	next stream item
//              :   @fsDEs_s	:	stream context structure
// return       :   
// notes        :   
//-----------------------------------------------------------------------------------
uint32_t fs_stream_dec_step (uint32_t next, struct fs_stream_context *fsDEs_s)
 {
	fs_stream_next_sub_key(fsDEs_s);
	fsDEs_s->dec=next^(fsDEs_s->key);
	fs_stream_next_key(fsDEs_s);
	return fsDEs_s->dec;
}

//-----------------------------------------------------------------------------------
// Function     :   fs_stream_enc_step
// Description  :   starts encryption process by getting next stream item
// Arguments    :   @next	:	next stream item
//              :   @fsDEs_s	:	stream context structure
// return       :   
// notes        :   
//-----------------------------------------------------------------------------------
uint32_t fs_stream_enc_step (uint32_t next, struct fs_stream_context *fsDEs_s)
 {
	fs_stream_next_sub_key(fsDEs_s);
	fsDEs_s->dec=next;
	fsDEs_s->enc=next^fsDEs_s->key;
	fs_stream_next_key(fsDEs_s);
	return fsDEs_s->enc;
}
