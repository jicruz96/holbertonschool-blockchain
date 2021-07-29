#include <blockchain.h>
#include <unistd.h>

/**
 * blockchain_destroy - deletes an existing Blockchain, along with all the Blocks it contains
 * @blockchain: pointer to blockchain structure to delete
 **/
void blockchain_destroy(blockchain_t *blockchain)
{
	llist_destroy(blockchain->chain, true, (node_dtor_t)block_destroy);
	free(blockchain);
}
