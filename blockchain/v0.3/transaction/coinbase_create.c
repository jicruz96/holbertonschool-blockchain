#include "transaction.h"
#include <stdlib.h>
#include <string.h>

/**
 * coinbase_create - creates a coinbase transaction
 * @receiver: public key of miner who will receive coinbase coins
 * @block_index: index of the Block the coinbase transaction will belong to
 * Return: pointer to created transaction or NULL on failure
 **/
transaction_t *coinbase_create(EC_KEY const *receiver, uint32_t block_index)
{
	transaction_t *coinbase = NULL;
	tx_in_t *input = NULL;
	tx_out_t *output = NULL;
	uint8_t pub[EC_PUB_LEN];

	if (
		!receiver                                                            ||
		!(coinbase = calloc(1, sizeof(transaction_t)))                       ||
		!(input = calloc(1, sizeof(tx_in_t)))                                ||
		!(output = tx_out_create(COINBASE_AMOUNT, ec_to_pub(receiver, pub))) ||
		!(coinbase->inputs = llist_create(MT_SUPPORT_FALSE))                  ||
		llist_add_node(coinbase->inputs, input, ADD_NODE_REAR) == -1         ||
		!(coinbase->outputs = llist_create(MT_SUPPORT_FALSE))                 ||
		llist_add_node(coinbase->outputs, output, ADD_NODE_REAR) == -1       ||
		!memcpy(input->tx_out_hash, &block_index, sizeof(block_index))       ||
		!transaction_hash(coinbase, coinbase->id)
	)
	{
		if (coinbase)
		{
			llist_destroy(coinbase->outputs, 0, NULL);
			llist_destroy(coinbase->inputs, 0, NULL);
		}
		free(output);
		free(input);
		free(coinbase);
		return (NULL);
	}

	return (coinbase);
}
