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

#ifndef __FSDESCIPHER_INIT_H__
#define __FSDESCIPHER_INIT_H__

void fs_cipher_init
 (size_t block_size, uint32_t key, uint32_t sub_key,
  uint8_t *key_table, struct fs_cipher_context *fsDEs_c);
  
void fs_cipher_init_key_file
 (size_t block_size, char *password,
  char *key_file_name, struct fs_cipher_context *fsDEs_c);
  
void fs_cipher_init_token
 (size_t block_size, char *password,
  uint32_t *token, struct fs_cipher_context *fsDEs_c);
  
#endif //__FSDESCIPHER_INIT_H__
