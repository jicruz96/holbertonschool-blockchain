#include <blockchain.h>
#include <string.h>

/**
 * is_genesis_block - checks if a block is the Genesis block
 * @block: pointer to block
 * Return: 1 if it is, 0 if it isn't
 **/
static int is_genesis_block(block_t const *block)
{
	return (
		block->data.len == sizeof(GENESIS_DATA) - 1 &&
		!memcmp(block->data.buffer, GENESIS_DATA, block->data.len) &&
		!block->info.index &&
		!block->info.difficulty &&
		!block->info.nonce &&
		block->info.timestamp == GENESIS_TIME &&
		!block->info.prev_hash
	);
}

/**
 * block_is_valid - verifies that a Block is valid
 * @block: Block to check
 * @prev_block: previous Block in the Blockchain, or NULL if block is first one
 * Return: 1 if valid | 0 if not
 **/
int block_is_valid(block_t const *block, block_t const *prev_block)
{
	uint8_t tmp[SHA256_DIGEST_LENGTH];

	if (!block)
		return (0);

	if (!block->info.index && (prev_block || !is_genesis_block(block)))
		return (0);

	if (!prev_block)
		return (0);

	if (block->info.index != prev_block->info.index + 1)
		return (0);

	if (block->data.len > BLOCKCHAIN_DATA_MAX)
		return (0);

	if (!block_hash(block, tmp))
		return (0);

	if (memcmp(tmp, block->hash, sizeof(tmp)))
		return (0);

	if (!block_hash(prev_block, tmp))
		return (0);

	if (memcmp(tmp, block->info.prev_hash, sizeof(tmp)))
		return (0);

	return (1);
}
