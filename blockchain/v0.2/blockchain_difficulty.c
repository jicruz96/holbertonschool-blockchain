#include <blockchain.h>
#include <stdio.h>

/*
Difficulty adjustment:
Retrieve the last Block for which an adjustment was made (the Block with index <blockchain_size> - DIFFICULTY_ADJUSTMENT_INTERVAL)
Compute the expected elapsed time between the two Blocks
Compute the actual elapsed time
The difficulty must be incremented if the elapsed time is lower than half the expected elapsed time
The difficulty must be decremented if the elapsed time is greater than twice the expected elapsed time
The difficulty should not change otherwise
*/


void adjust_difficulty(blockchain_t const *blockchain, block_t *last_block, uint32_t *difficulty)
{
	block_t *last_adjusted_block = llist_get_node_at(blockchain->chain, last_block->info.index - DIFFICULTY_ADJUSTMENT_INTERVAL);
	uint64_t actual_time = last_block->info.timestamp - last_adjusted_block->info.timestamp;

	fprintf(stderr, "[%u] E: %d | A: %lu | lbts: %lu | labits: %lu\n", last_block->info.index, EXPECTED_TIME_BETWEEN_ADJUSTMENTS, actual_time, last_block->info.timestamp, last_adjusted_block->info.timestamp);

	if (actual_time <= EXPECTED_TIME_BETWEEN_ADJUSTMENTS / 2)
		*difficulty += 1;
	else if (*difficulty && actual_time >= EXPECTED_TIME_BETWEEN_ADJUSTMENTS * 2)
		*difficulty -= 1;
}

/**
 * blockchain_difficulty - computes difficulty to assign to next block in chain
 * @blockchain: pointer to Blockchain to analyze
 * Return: difficulty to be assigned to a potential next block
 **/
uint32_t blockchain_difficulty(blockchain_t const *blockchain)
{
	block_t *block;
	uint32_t difficulty;

	if (!blockchain || !(block = llist_get_tail(blockchain->chain)))
		return (0);

	difficulty = block->info.difficulty;

	if (block->info.index && !(block->info.index % DIFFICULTY_ADJUSTMENT_INTERVAL))
		adjust_difficulty(blockchain, block, &difficulty);

	return (difficulty);
}
