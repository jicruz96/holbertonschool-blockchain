#include <blockchain.h>
#include <unistd.h>
#include <stdio.h>

/**
 * blockchain_destroy - deletes an existing Blockchain, along with all the Blocks it contains
 * @blockchain: pointer to blockchain structure to delete
 **/
void blockchain_destroy(blockchain_t *blockchain)
{

	llist_destroy(blockchain->chain, true, (node_dtor_t)block_destroy);
	llist_destroy(blockchain->unspent, true, NULL);
	free(blockchain);
}
