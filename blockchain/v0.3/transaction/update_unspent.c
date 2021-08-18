#include "transaction.h"
#include <string.h>

/**
 * find_unspent - finds an unspent transaction output from a list of
 *                unspent transaction outputs
 * @node: an unspent transaction output
 * @hash: hash of transaction output to remove
 * Return: 1 on successful removal, 0 if hash not found or error occured
 **/
int find_unspent(llist_node_t node, void *hash)
{
	unspent_tx_out_t *unspent = (unspent_tx_out_t *)node;

	return (!memcmp(unspent->out.hash, hash, sizeof(unspent->out.hash)));
}

/**
 * update_unspent - updates the list of unspent transaction outputs, given a
 *                  list of processed transactions
 * @transactions: list of validated transactions
 * @block_hash: hash of the validated block that contains `transactions`
 * @all_unspent: list of unspent transaction outputs.
 * Return: updated list of unspent transaction outputs
 **/
llist_t *update_unspent(llist_t *transactions, uint8_t block_hash[SHA256_DIGEST_LENGTH], llist_t *all_unspent)
{
	int i, j, tmp, size;
	transaction_t *tx;
	tx_in_t *in;
	tx_out_t *out;
	unspent_tx_out_t *unspent;

	if (!transactions || !block_hash || !all_unspent)
		return (all_unspent);

	for (i = 0, size = llist_size(transactions); i < size; i++)
	{
		tx = llist_get_node_at(transactions, i);
		for (j = 0, tmp = llist_size(tx->inputs); j < tmp; j++)
		{
			in = llist_get_node_at(tx->inputs, j);
			llist_remove_node(all_unspent, find_unspent, in->tx_out_hash, 1, NULL);
		}
		for (j = 0, tmp = llist_size(tx->outputs); j < tmp; j++)
		{
			out = llist_get_node_at(tx->outputs, j);
			unspent = unspent_tx_out_create(block_hash, tx->id, out);
			llist_add_node(all_unspent, unspent, ADD_NODE_REAR);
		}
	}

	return (all_unspent);
}
