#include <blockchain.h>
#include <string.h>



/**
 * blockchain_create - creates a Blockchain structure, and initializes it
 *                     it must contain one block upon creation. This block is
 *                     called the Genesis Block. Its content is static, and
 *                     pre-defined as follows:
 *                         index -> 0
 *                         difficulty -> 0
 *                         timestamp -> 1537578000
 *                         nonce -> 0
 *                         prev_hash -> 0 (times 32)
 *                         data -> "Holberton School" (16 bytes long)
 *                         hash -> c52c26c8b5461639635d8edf2a97d48d0c8e0009c817f2b1d3d7ff2f04515803
 * Return: pointer to blockchain struct
 **/
blockchain_t *blockchain_create(void)
{
	blockchain_t *blockchain = malloc(sizeof(blockchain_t));
	block_t *genesis;

	if (!blockchain)
		return (NULL);

	genesis = malloc(sizeof(block_t));

	if (!genesis)
	{
		free(blockchain);
		return (NULL);
	}

	blockchain->chain = llist_create(MT_SUPPORT_FALSE);

	if (!blockchain->chain)
	{
		free(blockchain);
		free(genesis);
		return (NULL);
	}

	genesis->info.index = 0;
	genesis->info.difficulty = 0;
	genesis->info.timestamp = GENESIS_TIME;
	genesis->info.nonce = 0;
	memset(genesis->info.prev_hash, 0, sizeof(genesis->info.prev_hash));
	memcpy(genesis->data.buffer, GENESIS_DATA, sizeof(GENESIS_DATA) - 1);
	genesis->data.len = sizeof(GENESIS_DATA) - 1;
	memcpy(genesis->hash, GENESIS_HASH, sizeof(GENESIS_HASH));

	if (llist_add_node(blockchain->chain, genesis, ADD_NODE_FRONT) == -1)
	{
		llist_destroy(blockchain->chain, false, NULL);
		free(genesis);
		free(blockchain);
		return (NULL);
	}

	return (blockchain);
}
