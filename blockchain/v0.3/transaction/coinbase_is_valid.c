#include "transaction.h"
#include <string.h>
#include <stdio.h>

/**
 * is_empty - checks if a memory space is zero'd
 * @p: pointer to memory space
 * @size: size of space to check
 * Return: 1 if empty, else 0
 **/
static int is_empty(void *p, size_t size)
{
	while (size--)
		if (*((char *)p))
			return (0);

	return (1);
}

/**
 * coinbase_is_valid - checks wheter a coinbase transaction is valid
 * @coinbase: coinbase transaction to verify
 * @block_index: index of Block coinbase will belong to
 * Return: 1 if coinbase is valid | 0 if not
 **/
int coinbase_is_valid(transaction_t const *coinbase, uint32_t block_index)
{
	uint8_t hash_buf[SHA256_DIGEST_LENGTH];
	tx_in_t *tmp_input;
	tx_out_t *tmp_output;

	return (
		coinbase != NULL                                                   &&
		transaction_hash(coinbase, hash_buf) != NULL                       &&
		memcmp(hash_buf, coinbase->id, sizeof(hash_buf)) == 0              &&
		llist_size(coinbase->outputs) == 1                                 &&
		llist_size(coinbase->inputs) == 1                                  &&
		(tmp_input = llist_get_head(coinbase->inputs))                     &&
		!memcmp(tmp_input->tx_out_hash, &block_index, sizeof(block_index)) &&
		is_empty(tmp_input->block_hash, sizeof(tmp_input->block_hash))     &&
		is_empty(tmp_input->tx_id, sizeof(tmp_input->tx_id))               &&
		is_empty(&tmp_input->sig, sizeof(tmp_input->sig))                  &&
		(tmp_output = llist_get_head(coinbase->outputs))                   &&
		tmp_output->amount == COINBASE_AMOUNT
	);
}
