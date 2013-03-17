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

#ifndef __FSDESCOMMON_H__
#define __fsDEsCommon_h__

void block_8_to_32 (uint8_t *bl8, uint32_t *bl32, size_t size);
 
void block_32_to_8 (uint8_t *bl8, uint32_t *bl32, size_t size);
 
size_t fsize (FILE *in);
 
uint8_t * fs_read_key_file (uint8_t *key_file_name);
 
void fs_gen_key_file (char *key_file_name, uint32_t seed);

void fs_cipher_calc_key_pair (char *password, uint32_t *key, uint32_t *sub_key);
 
uint8_t * fs_cipher_token (uint32_t *token);

 #endif //__FSDESCOMMON_H__
 