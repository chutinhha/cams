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
 
#define SAFE_FILE_ENC 1 // 8 random bytes at start of file
#define OLD_FILE_ENC 0 // old style

struct fs_cipher_context{
	size_t block_size; // 32-bit blocks
	uint32_t key;
	uint32_t sub_key;
	uint8_t *table; // 64k table
	uint8_t flag;
};

uint32_t fs_cipher_modify
 (uint32_t a, uint32_t b, struct fs_cipher_context *fsDEs_c);
 
void fs_cipher_block_encipher
 (uint32_t *block, struct fs_cipher_context *fsDEs_c);
 
void fs_cipher_block_decipher
 (uint32_t *block, struct fs_cipher_context *fsDEs_c);
 
void fs_cipher_file_encipher
 (char *input_file_name, char *output_file_name, struct fs_cipher_context *fsDEs_c);
 
void fs_cipher_file_decipher
 (char *input_file_name, char *output_file_name, struct fs_cipher_context *fsDEs_c);
