/***************************************************************************
 ** File            :   
 ** 
 ** Description     :   
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
#include <stdlib.h>
#include <stdio.h>

#include "fsDEsCipher.h"
#include "fsDEsRandom.h"

//-----------------------------------------------------------------------------------
// Function     :   fs_cipher_modify
// Description  :   modifies the cipher key value
// Arguments    :   @a			-	key value
//                  @b			-	sub key value
//					@fsDEs_c	-	cipher context structure
// return       :   key value changed
// notes        :   
//-----------------------------------------------------------------------------------
uint32_t fs_cipher_modify (uint32_t a, uint32_t b, struct fs_cipher_context *fsDEs_c)
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
	
	k[0]=fsDEs_c->table[k[0]*256+m[0]];
	k[1]=fsDEs_c->table[k[1]*256+m[1]];
	k[2]=fsDEs_c->table[k[2]*256+m[2]];
	k[3]=fsDEs_c->table[k[3]*256+m[3]];
	
	return (k[3]<<24)+(k[0]<<16)+(k[1]<<8)+(k[2]);
 }

//-----------------------------------------------------------------------------------
// Function     :   fs_cipher_block_encipher
// Description  :   modifies the cipher key value during encryption
// Arguments    :   @block		-	block value
//					@fsDEs_c	-	cipher context structure
// return       :   none
// notes        :   
//-----------------------------------------------------------------------------------
void fs_cipher_block_encipher (uint32_t *block, struct fs_cipher_context *fsDEs_c)
 {
	size_t i;
	uint32_t tmp;

	fsDEs_c->key = fs_cipher_modify
		(fsDEs_c->key, fsDEs_c->sub_key, fsDEs_c);
		
	for(i=0; i < fsDEs_c->block_size; i++){
		fsDEs_c->sub_key = fs_cipher_modify
			(fsDEs_c->sub_key, fsDEs_c->key | fsDEs_c->sub_key, fsDEs_c);
		tmp = block[i];
		block[i] ^= fsDEs_c->key;
		fsDEs_c->key = fs_cipher_modify
			(fsDEs_c->key, tmp & fsDEs_c->sub_key, fsDEs_c);
	}
 }

//-----------------------------------------------------------------------------------
// Function     :   fs_cipher_block_decipher
// Description  :   modifies the cipher key value during decryption
// Arguments    :   @block		-	block value
//					@fsDEs_c	-	cipher context structure
// return       :   none
// notes        :   
//-----------------------------------------------------------------------------------
 void fs_cipher_block_decipher
 (uint32_t *block, struct fs_cipher_context *fsDEs_c)
 {
	size_t i;

	fsDEs_c->key = fs_cipher_modify
		(fsDEs_c->key, fsDEs_c->sub_key, fsDEs_c);
		
	for(i=0; i < fsDEs_c->block_size; i++){
		fsDEs_c->sub_key = fs_cipher_modify
			(fsDEs_c->sub_key, fsDEs_c->key | fsDEs_c->sub_key, fsDEs_c);
		block[i] ^= fsDEs_c->key;
		fsDEs_c->key = fs_cipher_modify
			(fsDEs_c->key, block[i] & fsDEs_c->sub_key, fsDEs_c);
	}
 }
//Block ciper end

//-----------------------------------------------------------------------------------
// Function     :   fs_cipher_file_encipher
// Description  :   File encryption
// Arguments    :   @block		-	input file
//					@fsDEs_c	-	output file
//                  @gsDES_c    -   cipher contect structure
// return       :   none
// notes        :   
//-----------------------------------------------------------------------------------
void fs_cipher_file_encipher (char *input_file_name, char *output_file_name, struct fs_cipher_context *fsDEs_c)
 {
	FILE *input_file, *output_file;
	size_t file_size, i, block_num, block_bytes;
	uint8_t *block_8;
	uint32_t *block_32;
	uint8_t offset=0; //for SAFE_FILE_ENC
	
	
	block_bytes = fsDEs_c->block_size * sizeof(uint32_t); // for fread
	
	// open input file
	input_file = fopen(input_file_name, "rb");
	if (input_file == NULL)
    {
		perror(input_file_name);
		exit(1);
	}
	
    // open output file
	output_file = fopen(output_file_name, "wb");
	if (output_file == NULL){
		perror(output_file_name);
		exit(1);
	}
	
	//malloc
	block_8 = (uint8_t *) malloc (block_bytes);
	if(block_8 == NULL){
		perror("fs_cipher_file_encipher:malloc_block_8:");
		exit(1);
	}
	
	block_32 = (uint32_t *) malloc (block_bytes);
	if(block_32 == NULL){
		perror("fs_cipher_file_encipher:malloc_block_32:");
		exit(1);
	}

	//Get file _size
	file_size = fsize(input_file);
	
	if(fsDEs_c->flag == SAFE_FILE_ENC){// for 8 random bytes
		offset=8;
		file_size+=8;
	}

	if(file_size%block_bytes==0)
	{
        block_num = file_size / block_bytes;
    }
	else
    {
        block_num = (file_size / block_bytes) + 1;
    }

	for(i=0; i<block_num; i++)
    {

		if(fsDEs_c->flag == SAFE_FILE_ENC && i == 1)
			offset=0;

		if(file_size%block_bytes!=0 && i==block_num-1)
        {
			fread(block_8+offset, 1, (file_size-offset)%block_bytes, input_file);
        }
		else
        {
            fread(block_8+offset, 1, block_bytes-offset, input_file);
        }

		block_8_to_32(block_8, block_32, block_bytes);
		
		if(fsDEs_c->flag == SAFE_FILE_ENC)
			if(i==0){
				block_32[0] = fs_random_32();
				block_32[1] = fs_random_32();
			}
	
		fs_cipher_block_encipher(block_32, fsDEs_c);
		
		block_32_to_8(block_8, block_32, block_bytes);
		
		if(file_size%block_bytes!=0 && i==block_num-1)
		{
            fwrite(block_8, 1, file_size%block_bytes, output_file);
        }
		else
        {
            fwrite(block_8, 1, block_bytes, output_file);
        }
	}
	free(block_8);
	free(block_32);
	fclose(input_file);
	fclose(output_file);
 }
 
//-----------------------------------------------------------------------------------
// Function     :   fs_cipher_file_encipher
// Description  :   File encryption
// Arguments    :   @block		-	input file
//					@fsDEs_c	-	output file
//                  @gsDES_c    -   cipher contect structure
// return       :   none
// notes        :   
//----------------------------------------------------------------------------------- 
void fs_cipher_file_decipher (char *input_file_name, char *output_file_name, struct fs_cipher_context *fsDEs_c)
 {
	FILE *input_file, *output_file;
	size_t file_size, i, block_num, block_bytes;
	uint8_t *block_8;
	uint32_t *block_32;
	uint8_t offset=0; //for SAFE_FILE_ENC

	block_bytes = fsDEs_c->block_size * sizeof(uint32_t); // for fread
	
	// File open
	input_file = fopen(input_file_name, "rb");
	if (input_file == NULL){
		perror(input_file_name);
		exit(1);
	}
	
	output_file = fopen(output_file_name, "wb");
	if (output_file == NULL){
		perror(output_file_name);
		exit(1);
	}
	
	//malloc
	block_8 = (uint8_t *) malloc (block_bytes);
	if(block_8 == NULL){
		perror("fs_cipher_file_decipher:malloc_block_8:");
		exit(1);
	}
	
	block_32 = (uint32_t *) malloc (block_bytes);
	if(block_32 == NULL){
		perror("fs_cipher_file_decipher:malloc_block_32:");
		exit(1);
	}

	//Get file _size
	file_size = fsize(input_file);
	
	if(fsDEs_c->flag == SAFE_FILE_ENC){// for 8 random bytes
		offset=8;
	}

	if(file_size%block_bytes==0)
		block_num = file_size / block_bytes;
	else block_num = (file_size / block_bytes) + 1;

	for(i=0; i<block_num; i++){

		if(fsDEs_c->flag == SAFE_FILE_ENC && i == 1)
			offset=0;

		if(file_size%block_bytes!=0 && i==block_num-1)
			fread(block_8, 1, file_size%block_bytes, input_file);
		else fread(block_8, 1, block_bytes, input_file);

		block_8_to_32(block_8, block_32, block_bytes);
	
		fs_cipher_block_decipher(block_32, fsDEs_c);
		
		block_32_to_8(block_8, block_32, block_bytes);
		
		if(file_size%block_bytes!=0 && i==block_num-1)
			fwrite(block_8+offset, 1, (file_size-offset)%block_bytes, output_file);
		else fwrite(block_8+offset, 1, block_bytes-offset, output_file);
	}
	free(block_8);
	free(block_32);
	fclose(input_file);
	fclose(output_file);
 }
