#include "transaction.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

void _print_hex_buffer(uint8_t const *buf, size_t len);

/**
 * tx_in_sign - signs a transaction input, given the transaction id it's from
 * @in: transaction input structure to sign
 * @tx_id: transaction ID (hash) of the transaction `in` is stored in
 * @sender: private key of coin recipient contained in the transaction output
 *          referenced by `in`
 * @all_unspent: list of all unspent transaction outputs to date
 * Return: pointer to a signature structure or NULL on failure
 **/
sig_t *tx_in_sign(tx_in_t *in, uint8_t const tx_id[SHA256_DIGEST_LENGTH], EC_KEY const *sender, llist_t *all_unspent)
{
	uint8_t pub[EC_PUB_LEN];
	unsigned int i, tmp;
	unspent_tx_out_t *tmp_unspent;


	ec_to_pub(sender, pub);

	tmp = llist_size(all_unspent);
	for (i = 0; i < tmp; i++)
	{
		tmp_unspent = llist_get_node_at(all_unspent, i);
		if (!memcmp(in->tx_out_hash, tmp_unspent->out.hash, sizeof(in->tx_out_hash)))
			break;
	}

	if (i == tmp || memcmp(pub, tmp_unspent->out.pub, sizeof(pub)))
		return (NULL);

	ec_sign(sender, tx_id, SHA256_DIGEST_LENGTH, &in->sig);
	return (&in->sig);
}
