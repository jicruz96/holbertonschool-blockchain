#include "transaction.h"
#include "../blockchain.h"
#include <stdlib.h>
#include <string.h>

#define tx_out_hash(tx) sha256((const int8_t *)(tx), sizeof((tx)->amount) + sizeof((tx)->pub), (tx)->hash)


/**
 * tx_out_create - creates a new transcation output structure
 * @amount: transaction amount
 * @pub: public key of transaction recipient
 * Return: pointer to transaction output structure
 **/
tx_out_t *tx_out_create(uint32_t amount, uint8_t const pub[EC_PUB_LEN])
{
	tx_out_t *tx = malloc(sizeof(tx_out_t));

	if (tx)
	{
		tx->amount = amount;
		memcpy(tx->pub, pub, sizeof(tx->pub));
		tx_out_hash(tx);
	}
	return (tx);
}
