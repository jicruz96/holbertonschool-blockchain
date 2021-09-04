#ifndef _TRANSACTION_H_
#define _TRANSACTION_H_

#include <llist.h>
#include <hblk_crypto.h>

#define COINBASE_AMOUNT 50


/* Transaction structures */

/**
 * struct tx_out_s - Transaction output structure
 * @amount: amount received
 * @pub:    public key of receiver
 * @hash:   hash of @amount and @pub. Serves as output ID
 **/
typedef struct tx_out_s
{
	uint32_t amount;
	uint8_t pub[EC_PUB_LEN];
	uint8_t hash[SHA256_DIGEST_LENGTH];
} tx_out_t;

/**
 * struct tx_in_s - Transaction input structure
 *
 * Description: A transaction input always refers to a previous transaction
 * output. The only exception is for a Coinbase transaction that adds new coins
 * to circulation.
 *
 * @block_hash: hash of block containing transaction referenced by @tx_id
 * @tx_id: ID of transaction containing @tx_out_hash
 * @tx_out_hash: hash of the referenced transaction output
 * @sig: signature. Prevents anyone from altering the content of the transaction
 **/
typedef struct tx_in_s
{
	uint8_t block_hash[SHA256_DIGEST_LENGTH];
	uint8_t tx_id[SHA256_DIGEST_LENGTH];
	uint8_t tx_out_hash[SHA256_DIGEST_LENGTH];
	sig_t sig;
} tx_in_t;

/**
 * struct unspent_tx_out_s - unspent transaction output
 * @block_hash: hash of the block containing @tx_id
 * @tx_id: ID of the transaction containing @out
 * @out: Copy of the referenced transaction output
 */
typedef struct unspent_tx_out_s
{
	uint8_t block_hash[SHA256_DIGEST_LENGTH];
	uint8_t tx_id[SHA256_DIGEST_LENGTH];
	tx_out_t out;
} unspent_tx_out_t;

/**
 * struct transaction_s - Transaction structure
 * @id:      Transaction ID. A hash of all inputs and outputs.
 *           Prevents further alteration of the transaction
 * @inputs:  List of `tx_in_t *` transaction inputs
 * @outputs: List of `unspent_tx_out_t *` transaction outputs
 **/
typedef struct transaction_s
{
	uint8_t id[SHA256_DIGEST_LENGTH];
	llist_t *inputs;
	llist_t *outputs;
} transaction_t;


tx_out_t *tx_out_create(uint32_t amount, uint8_t const pub[EC_PUB_LEN]);
unspent_tx_out_t *unspent_tx_out_create(uint8_t block_hash[SHA256_DIGEST_LENGTH], uint8_t tx_id[SHA256_DIGEST_LENGTH], tx_out_t const *out);
tx_in_t *tx_in_create(unspent_tx_out_t const *unspent);
uint8_t *transaction_hash(transaction_t const *transaction, uint8_t hash_buf[SHA256_DIGEST_LENGTH]);
sig_t *tx_in_sign(tx_in_t *in, uint8_t const tx_id[SHA256_DIGEST_LENGTH], EC_KEY const *sender, llist_t *all_unspent);
transaction_t *transaction_create(EC_KEY const *sender, EC_KEY const *receiver, uint32_t amount, llist_t *all_unspent);
int transaction_is_valid(transaction_t *transaction, llist_t *all_unspent);
transaction_t *coinbase_create(EC_KEY const *receiver, uint32_t block_index);
int coinbase_is_valid(transaction_t const *coinbase, uint32_t block_index);
void transaction_destroy(transaction_t *transaction);
llist_t *update_unspent(llist_t *transactions, uint8_t block_hash[SHA256_DIGEST_LENGTH], llist_t *all_unspent);



#endif /* _TRANSACTION_H_ */
