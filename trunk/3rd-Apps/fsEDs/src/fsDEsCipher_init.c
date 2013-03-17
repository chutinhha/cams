/***************************************************************************
 ** File            :   fsDEsCipher_init.c
 ** 
 ** Description     :   cipher initialization fucntions
 **
 ** History
 **     Created     :   Nov 5  2012
 **     Modified    :	Nov 16 2012
 **
 ** Notes           :	
 **
 ** ToDo            :
 **
 ***************************************************************************/

#include <stdint.h>
#include <stdio.h>

#include "fsDEsCipher.h"
#include "fsDEsCipher_init.h"
#include "fsDEsCommon.h"

void fs_cipher_init
 (size_t block_size, uint32_t key, uint32_t sub_key,
  uint8_t *key_table, struct fs_cipher_context *fsDEs_c)
 {
	fsDEs_c->block_size = block_size;
	fsDEs_c->key = key;
	fsDEs_c->sub_key = sub_key;
	fsDEs_c->table = key_table;
	fsDEs_c->flag = SAFE_FILE_ENC;
 }
 
void fs_cipher_init_key_file
 (size_t block_size, char *password,
  char *key_file_name, struct fs_cipher_context *fsDEs_c)
 {
	uint32_t key, sub_key;
	uint8_t *key_table;
	
	fs_cipher_calc_key_pair(password, &key, &sub_key);
	key_table = fs_read_key_file(key_file_name);
	
	fs_cipher_init(block_size, key, sub_key, key_table, fsDEs_c);
 }
 
void fs_cipher_init_token
 (size_t block_size, char *password,
  uint32_t *token, struct fs_cipher_context *fsDEs_c)
 {
	uint32_t key, sub_key;
	uint8_t *key_table;
	
	fs_cipher_calc_key_pair(password, &key, &sub_key);
	
	key_table = fs_cipher_token(token);

	fs_cipher_init(block_size, key, sub_key, key_table, fsDEs_c);
 }
 
 

