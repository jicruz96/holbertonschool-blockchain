#include <blockchain.h>
#include <unistd.h>

/**
 * block_destroy - deletes an existing block
 * @block: block to destroy
 **/
void block_destroy(block_t *block)
{
	free(block);
}
