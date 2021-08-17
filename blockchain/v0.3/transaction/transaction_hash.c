#include "transaction.h"
#include <string.h>

/**
 * transaction_hash - computes the ID (hash) of a transaction
 * @transaction: transaction to compute hash of
 * @hash_buf: buffer where computed hash is stored
 * Return: pointer to `hash_buf`
 **/
uint8_t *transaction_hash(transaction_t const *transaction, uint8_t hash_buf[SHA256_DIGEST_LENGTH])
{
	size_t i, num_inputs, num_outputs, size = 0;
	int8_t *buf;
	tx_in_t *tmp_in;
	tx_out_t *tmp_out;

	num_inputs  = llist_size(transaction->inputs);
	num_outputs = llist_size(transaction->outputs);

	if (!(buf = calloc(1, SHA256_DIGEST_LENGTH * (3 * num_inputs + num_outputs))))
		return (NULL);


	for (i = 0; i < num_inputs; i++)
	{
		tmp_in = llist_get_node_at(transaction->inputs, i);
		memcpy(buf + size, tmp_in, sizeof(*tmp_in) - sizeof(tmp_in->sig));
		size += sizeof(*tmp_in) - sizeof(tmp_in->sig);
	}

	for (i = 0; i < num_outputs; i++)
	{
		tmp_out = llist_get_node_at(transaction->outputs, i);
		memcpy(buf + size, tmp_out->hash, sizeof(tmp_out->hash));
		size += sizeof(tmp_out->hash);
	}

	if (!sha256(buf, size, hash_buf))
	{
		free(buf);
		return (NULL);
	}

	free(buf);
	return (hash_buf);
}
