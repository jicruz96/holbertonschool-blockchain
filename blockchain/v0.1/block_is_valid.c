#include <blockchain.h>

/*
Write a function that verifies that a Block is valid

Prototype: int block_is_valid(block_t const *block, block_t const *prev_block);, where:
block points to the Block to check
prev_block points to the previous Block in the Blockchain, or is NULL if block is the first Block of the chain
The following requirements must be fulfilled for a Block to be valid:
block should not be NULL
prev_block should be NULL ONLY if block‘s index is 0
If the Block’s index is 0, the Block should match the Genesis Block described in the first task
The Block’s index must be the previous Block’s index, plus 1
The computed hash of the previous block must match the one it stores
The computed hash of the previous block must match the reference one stored in block
The computed hash of the Block must match the one it stores
You don’t have to worry about the timestamp and the difficulty for this iteration of the Blockchain.
The Block data length must not exceed BLOCKCHAIN_DATA_MAX

*/
