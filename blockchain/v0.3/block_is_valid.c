#include <blockchain.h>
#include <string.h>

/**
 * is_genesis_block - checks if a block is the Genesis block
 * @block: pointer to block
 * Return: 1 if it is, 0 if it isn't
 **/
static int is_genesis_block(block_t const *block)
{
	static uint8_t buf[SHA256_DIGEST_LENGTH] = {0};

	return (
		block->data.len == sizeof(GENESIS_DATA) - 1 &&
		!memcmp(block->data.buffer, GENESIS_DATA, block->data.len) &&
		!block->info.index &&
		!block->info.difficulty &&
		!block->info.nonce &&
		block->info.timestamp == GENESIS_TIME &&
		!memcmp(block->info.prev_hash, buf, sizeof(buf))
	);
}

/**
 * block_transactions_are_valid - checks if a block's transactions are valid
 * @block: pointer to block
 * @all_unspent: list of utxo's
 * Return: 1 if transactions are valid, 0 otherwise
 **/
static int block_transactions_are_valid(block_t const *block, llist_t *all_unspent)
{
	int i, size = llist_size(block->transactions);
	transaction_t *tx;

	if (!size || !block)
		return (0);

	for (i = 0; i < size; i++)
	{
		tx = llist_get_node_at(block->transactions, i);
		if ((i && !transaction_is_valid(tx, all_unspent)) || !coinbase_is_valid(tx, block->info.index))
			return (0);
	}

	return (1);
}

/**
 * block_is_valid - verifies that a Block is valid
 * @block: Block to check
 * @prev_block: previous Block in the Blockchain, or NULL if block is first one
 * @all_unspent: list of unspent transaction outputs in blockchain
 * Return: 1 if valid | 0 if not
 **/
int block_is_valid(block_t const *block, block_t const *prev_block, llist_t *all_unspent)
{
	uint8_t tmp[SHA256_DIGEST_LENGTH];

	if (!block)
		return (1);

	if (!block->info.index)
		return (prev_block || !is_genesis_block(block));

	if (!prev_block)
		return (1);

	if (block->info.index != prev_block->info.index + 1)
		return (1);

	if (block->data.len > BLOCKCHAIN_DATA_MAX)
		return (1);

	if (!block_hash(block, tmp))
		return (1);

	if (memcmp(tmp, block->hash, sizeof(tmp)))
		return (1);

	if (!hash_matches_difficulty(block->hash, block->info.difficulty))
		return (1);

	if (!block_hash(prev_block, tmp))
		return (1);

	if (memcmp(tmp, block->info.prev_hash, sizeof(tmp)))
		return (1);

	if (!block_transactions_are_valid(block, all_unspent))
		return (1);

	return (0);
}
