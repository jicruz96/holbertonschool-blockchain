#include <blockchain.h>

/**
 * blockchain_difficulty - computes difficulty to assign to next block in chain
 * @blockchain: pointer to Blockchain to analyze
 * Return: difficulty to be assigned to a potential next block
 **/
uint32_t blockchain_difficulty(blockchain_t const *blockchain)
{
/*
The following macros must be defined in your header file blockchain.h
    BLOCK_GENERATION_INTERVAL
        Value: 1 (Will be changed during correction)
        Defines how often (in seconds) a Block should be found
    DIFFICULTY_ADJUSTMENT_INTERVAL
        Value: 5 (Will be changed during correction)
        Defines how often (in Blocks) the difficulty should be adjusted
    If the latest Block’s index is a multiple of DIFFICULTY_ADJUSTMENT_INTERVAL, AND that it is not the Genesis Block, the difficulty must be adjusted. Otherwise, the difficulty of the latest Block in blockchain is returned
    Difficulty adjustment:
        Retrieve the last Block for which an adjustment was made (the Block with index <blockchain_size> - DIFFICULTY_ADJUSTMENT_INTERVAL)
        Compute the expected elapsed time between the two Blocks
        Compute the actual elapsed time
        The difficulty must be incremented if the elapsed time is lower than half the expected elapsed time
        The difficulty must be decremented if the elapsed time is greater than twice the expected elapsed time
        The difficulty should not change otherwise
*/
    return (0);
}
