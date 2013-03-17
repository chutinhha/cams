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
 
struct fs_stream_context{
	uint32_t enc;
	uint32_t dec;
	uint32_t key;
	uint32_t sub_key;
	uint8_t *table;
};

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
