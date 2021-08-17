#include "transaction.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

/**
 * transaction_init - constructs a transaction structure
 * @inputs: transaction inputs
 * @outputs: tansaction outputs
 * @sender: sender of transaction
 * @all_unspent: list of unspent transaction outputs, used for signature
 * Return: pointer to transaction or NULL on failure
 **/
transaction_t *transaction_init(llist_t *inputs, llist_t *outputs, EC_KEY const *sender, llist_t *all_unspent)
{
	transaction_t *transaction = malloc(sizeof(transaction_t));
	tx_in_t *tmp_input;
	int size, i;

	if (!transaction)
		return (NULL);

	transaction->inputs = inputs;
	transaction->outputs = outputs;
	transaction_hash(transaction, transaction->id);

	for (i = 0, size = llist_size(inputs); i < size; i++)
	{
		tmp_input = llist_get_node_at(inputs, i);
		if (!tx_in_sign(tmp_input, transaction->id, sender, all_unspent))
		{
			free(transaction);
			return (NULL);
		}
	}

	return (transaction);
}

/**
 * transaction_create - creates a transaction structure
 * @sender: private key of transaction sender
 * @receiver: public key of transaction receiver
 * @amount: amount to send
 * @all_unspent: list of all unspent outputs
 * Return: pointer to created transaction or NULL on failure
 **/
transaction_t *transaction_create(EC_KEY const *sender, EC_KEY const *receiver, uint32_t amount, llist_t *all_unspent)
{
	transaction_t *transaction = NULL;
	llist_t *outputs = NULL, *inputs = NULL;
	unspent_tx_out_t *tmp_unspent;
	uint32_t total;
	int i, tmp;
	tx_in_t *tmp_input;
	tx_out_t *tmp_output;
	uint8_t receiver_pub_key[EC_PUB_LEN], sender_pub_key[EC_PUB_LEN];

	if (!sender || !receiver || !amount || !all_unspent || !(tmp = llist_size(all_unspent)) ||  !ec_to_pub(sender, sender_pub_key) || !(inputs = llist_create(MT_SUPPORT_FALSE)))
		return (NULL);

	for (total = 0, i = 0; i < tmp && total < amount; i++)
	{
		tmp_unspent = llist_get_node_at(all_unspent, i);
		if (memcmp(sender_pub_key, tmp_unspent->out.pub, sizeof(sender_pub_key)))
			continue;
		if (
			!(tmp_input = tx_in_create(tmp_unspent)) ||
			llist_add_node(inputs, tmp_input, ADD_NODE_REAR) == -1
		)
		{
			llist_destroy(inputs, 1, free);
			return (NULL);
		}
		total += tmp_unspent->out.amount;
	}

	if
	(
		total < amount                                                ||
		!(outputs = llist_create(MT_SUPPORT_FALSE))                    ||
		!ec_to_pub(receiver, receiver_pub_key)                        ||
		!(tmp_output = tx_out_create(amount, receiver_pub_key))       ||
		llist_add_node(outputs, tmp_output, ADD_NODE_REAR) == -1      ||
		(
			(total -= amount) &&
			(
				!(tmp_output = tx_out_create(total, sender_pub_key))  ||
				llist_add_node(outputs, tmp_output, ADD_NODE_REAR) == -1
			)
		)                                                             ||
		!(transaction = transaction_init(inputs, outputs, sender, all_unspent))
	)
	{
		llist_destroy(inputs, 1, free);
		if (outputs)
			llist_destroy(outputs, 1, free);
		return (NULL);
	}

	return (transaction);
}
