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

struct fs_stream_context{
	uint32_t enc;
	uint32_t dec;
	uint32_t key;
	uint32_t sub_key;
	uint8_t *table;
};

struct fs_hash_context{
	uint8_t *table;
	uint32_t hash[8];
	uint8_t hash_len;
	uint8_t ent_num;
};

void fs_cipher_init
 (size_t block_size, uint32_t key, uint32_t sub_key,
  uint8_t *key_table, struct fs_cipher_context *fsDEs_c);
  
void fs_cipher_init_key_file
 (size_t block_size, char *password,
  char *key_file_name, struct fs_cipher_context *fsDEs_c);
  
void fs_cipher_init_token
 (size_t block_size, char *password,
  uint32_t *token, struct fs_cipher_context *fsDEs_c);

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

uint32_t fs_stream_modify
 (uint32_t a, uint32_t b, struct fs_stream_context *fsDEs_s);

void fs_stream_simple_init_key_file
 (uint8_t *password, uint8_t *key_file_name, struct fs_stream_context *fsDEs_s);

void fs_stream_simple_init_token
 (uint8_t *password, uint32_t *token, struct fs_stream_context *fsDEs_s);

void fs_stream_init
 (uint8_t *key_table, uint32_t key, uint32_t sub_key, struct fs_stream_context *fsDEs_s);

void fs_stream_next_key
 (struct fs_stream_context *fsDEs_s);

void fs_stream_next_sub_key
 (struct fs_stream_context *fsDEs_s);

uint32_t fs_stream_enc_step
 (uint32_t next, struct fs_stream_context *fsDEs_s);

uint32_t fs_stream_dec_step
 (uint32_t next, struct fs_stream_context *fsDEs_s);

void fs_srandom(uint32_t seed);

uint32_t fs_random_32();

uint64_t fs_random_64();

void fs_hash_init
 (struct fs_hash_context *fsDEsH);
 
void fs_hash_step
 (uint32_t next, struct fs_hash_context *fsDEsH);
 
uint32_t fs_hash_modify
 (uint32_t a, uint32_t b, struct fs_hash_context *fsDEsH);

uint32_t fs_hash_rotate
 (uint32_t a);
 
void fs_hash_calc
 (uint32_t *data, size_t data_len, struct fs_hash_context *fsDEsH);

void block_8_to_32
 (uint8_t *bl8, uint32_t *bl32, size_t size);
 
void block_32_to_8
 (uint8_t *bl8, uint32_t *bl32, size_t size);
 
size_t fsize
 (FILE *in);
 
uint8_t * fs_read_key_file
 (uint8_t *key_file_name);
 
void fs_gen_key_file
 (char *key_file_name, uint32_t seed);

void fs_cipher_calc_key_pair
 (char *password, uint32_t *key, uint32_t *sub_key);
 
uint8_t * fs_cipher_token
 (uint32_t *token);
