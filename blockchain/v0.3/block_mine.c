#include <blockchain.h>

/**
 * block_mine - mines a block in order to insert in into a blockchain
 * @block: block to be mined
 **/
void block_mine(block_t *block)
{
	if (!block)
		return;

	for (block->info.nonce = 0; 1; block->info.nonce += 1)
	{
		block_hash(block, block->hash);
		if (hash_matches_difficulty(block->hash, block->info.difficulty))
			break;
	}
}
