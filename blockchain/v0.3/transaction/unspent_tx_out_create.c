#include "transaction.h"
#include <stdlib.h>
#include <string.h>

/**
 * unspent_tx_out_create - creates an unspent transaction output structure
 * @block_hash: hash of block where referenced transaction output is located
 * @tx_id: hash of a transaction in block defined by block hash
 * @out: points to the referenced transaction output
 * Return: pointer to unspent transaction output structure
 **/
unspent_tx_out_t *unspent_tx_out_create(uint8_t block_hash[SHA256_DIGEST_LENGTH], uint8_t tx_id[SHA256_DIGEST_LENGTH], tx_out_t const *out)
{
	unspent_tx_out_t *tx = malloc(sizeof(unspent_tx_out_t));

	if (tx)
	{
		memcpy(tx->block_hash, block_hash, sizeof(tx->block_hash));
		memcpy(tx->tx_id, tx_id, sizeof(tx->tx_id));
		memcpy(&tx->out, out, sizeof(tx->out));
	}

	return (tx);
}
