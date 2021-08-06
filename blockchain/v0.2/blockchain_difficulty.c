#include <blockchain.h>

/**
 * blockchain_difficulty - computes difficulty to assign to next block in chain
 * @blockchain: pointer to Blockchain to analyze
 * Return: difficulty to be assigned to a potential next block
 **/
uint32_t blockchain_difficulty(blockchain_t const *blockchain)
{
	block_t *block, *last_adjusted_block;
	uint64_t expected_time, actual_time;

	if (!blockchain || !(block = llist_get_tail(blockchain->chain)))
		return (0);


	if (block->info.index == 0 || block->info.index % DIFFICULTY_ADJUSTMENT_INTERVAL)
		return (block->info.difficulty);

	last_adjusted_block = llist_get_node_at(blockchain->chain, block->info.index - (block->info.index % DIFFICULTY_ADJUSTMENT_INTERVAL));
	expected_time = BLOCK_GENERATION_INTERVAL * (block->info.index - last_adjusted_block->info.index);
	actual_time = block->info.timestamp - last_adjusted_block->info.timestamp;

	if (actual_time < expected_time / 2)
		return (block->info.difficulty + 1);

	if (actual_time > expected_time * 2)
		return (block->info.difficulty - 1);

	return (block->info.difficulty);
}
