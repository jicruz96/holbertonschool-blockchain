#include <blockchain.h>

/**
 * block_hash - computes the hash of a Block
 * @block: pointer to block to be hashed
 * @hash_buf: where hash will be stored
 * Return: pointer to hash_buf
 **/
uint8_t
*block_hash(block_t const *block, uint8_t hash_buf[SHA256_DIGEST_LENGTH])
{
	if (!block || !hash_buf)
		return (NULL);

	return (sha256((int8_t const *)block->hash, sizeof(block->hash), hash_buf));
}
