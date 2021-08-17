#include <blockchain.h>
#include <unistd.h>
#include <stdio.h>

/**
 * block_destroy - deletes an existing block
 * @block: block to destroy
 **/
void block_destroy(block_t *block)
{
	if (!block)
		return;
	llist_destroy(block->transactions, 1, (node_dtor_t)transaction_destroy);
	free(block);
}
