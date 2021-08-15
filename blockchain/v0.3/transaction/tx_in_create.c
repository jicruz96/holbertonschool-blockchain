#include "transaction.h"
#include <stdlib.h>
#include <string.h>
/**
 * tx_in_create - creates a transaction input structure
 * @unspent: unspent transation output. is converted to a transaction input
 * Return: pointer to transaction input structure
 **/
tx_in_t *tx_in_create(unspent_tx_out_t const *unspent)
{
	tx_in_t *tx = malloc(sizeof(tx_in_t));

	if (!tx || !unspent)
		return (NULL);

	memcpy(tx, unspent, sizeof(tx->block_hash) + sizeof(tx->tx_id));
	sha256((const int8_t *)&unspent->out, sizeof(unspent->out), tx->tx_out_hash);
	memset(&tx->sig, 0, sizeof(tx->sig));
	return (tx);
}
