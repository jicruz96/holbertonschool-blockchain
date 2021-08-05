
#include <blockchain.h>
#include <string.h>
#include <time.h>

/**
 * block_create - creates a Block structure and initializes it.
 * @prev:     is a pointer to the previous Block in the Blockchain
 * @data:     points to a memory area to duplicate in the Block’s data
 * @data_len: number of bytes to duplicate in data.
 *            * If data_len is bigger than BLOCKCHAIN_DATA_MAX, then only
 *            * BLOCKCHAIN_DATA_MAX bytes must be duplicated.
 * Return: pointer to the allocated block
 **/
block_t
*block_create(block_t const *prev, int8_t const *data, uint32_t data_len)
{
	block_t *block = calloc(1, sizeof(block_t));

	if (block)
	{
		if (data_len > BLOCKCHAIN_DATA_MAX)
			data_len = BLOCKCHAIN_DATA_MAX;
		memcpy(block->data.buffer, data, data_len);
		block->data.len = data_len;
		block->info.difficulty = 0;
		block->info.nonce = 0;
		time((time_t *)&block->info.timestamp);
		if (prev)
		{
			block->info.index = prev->info.index + 1;
			memcpy(block->info.prev_hash, prev->hash, sizeof(prev->hash));
		}
		else
		{
			block->info.index = 0;
			memset(block->info.prev_hash, 0, SHA256_DIGEST_LENGTH);
		}
		memset(block->hash, 0, sizeof(block->hash));
	}

	return (block);
}
