#ifndef _BLOCKCHAIN_
#define _BLOCKCHAIN_

#include <llist.h>
#include "../../crypto/hblk_crypto.h"
#define true 1
#define false 0
#define GENESIS_DATA "Holberton School"
#define GENESIS_TIME 1537578000
#define GENESIS_HASH \
"\
\xc5\x2c\x26\xc8\xb5\x46\x16\x39\
\x63\x5d\x8e\xdf\x2a\x97\xd4\x8d\
\x0c\x8e\x00\x09\xc8\x17\xf2\xb1\
\xd3\xd7\xff\x2f\x04\x51\x58\x03"
#define HBLK_MAGIC "HBLK"
#define HBLK_VERSION "0.3"
#define LSB 1
#define MSB 2
#define HBLK_ENDIAN LSB

/* blockchain difficulty macros */
#define BLOCK_GENERATION_INTERVAL 1 /* how often a block should be found (seconds) */
#define DIFFICULTY_ADJUSTMENT_INTERVAL 5 /* how often the difficulty should be adjusted (blocks) */
#define EXPECTED_TIME_BETWEEN_ADJUSTMENTS (BLOCK_GENERATION_INTERVAL * DIFFICULTY_ADJUSTMENT_INTERVAL)
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
 * @sig: signature. Prevens anyone from altering the content of the transaction
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

/* Block structures */

/**
 * struct blockchain_s - Blockchain structure
 * @chain:   Linked list of pointers to block_t
 * @unspent: Linked list of pointers to unspent_tx_out_t
 */
typedef struct blockchain_s
{
	llist_t     *chain;
	llist_t     *unspent;
} blockchain_t;


/**
 * struct block_info_s - Block info structure
 *
 * @index:      Index of the Block in the Blockchain
 * @difficulty: Difficulty of proof of work (hash leading zero bits)
 * @timestamp:  Time the Block was created at (UNIX timestamp)
 * @nonce:      Salt value used to alter the Block hash
 * @prev_hash:  Hash of the previous Block in the Blockchain
 */
typedef struct block_info_s
{
	/*
	 * The order of the elements in this structure should remain the same.
	 * It was designed so every element of this structure is aligned and
	 * doesn't require padding from the compiler.
	 * Therefore, it is possible to use the structure as an array of char,
	 * on any architecture.
	 */
	uint32_t    index;
	uint32_t    difficulty;
	uint64_t    timestamp;
	uint64_t    nonce;
	uint8_t     prev_hash[SHA256_DIGEST_LENGTH];
} block_info_t;

#define BLOCKCHAIN_DATA_MAX 1024

/**
 * struct block_data_s - Block data
 *
 * @buffer: Data buffer
 * @len:    Data size (in bytes)
 */
typedef struct block_data_s
{
	/*
	 * @buffer must stay first, so we can directly use the structure as
	 * an array of char
	 */
	int8_t      buffer[BLOCKCHAIN_DATA_MAX];
	uint32_t    len;
} block_data_t;

/**
 * struct block_s - Block structure
 *
 * @info:         Block info
 * @data:         Block data
 * @transactions: List of transactions
 * @hash:         256-bit digest of the Block, to ensure authenticity
 */
typedef struct block_s
{
	block_info_t    info; /* This must stay first */
	block_data_t    data; /* This must stay second */
	llist_t *transactions;
	uint8_t     hash[SHA256_DIGEST_LENGTH];
} block_t;

/* Blockchain creation and validation functions */
blockchain_t *blockchain_create(void);
block_t *block_create(block_t const *prev, int8_t const *data, uint32_t data_len);
void block_destroy(block_t *block);
void blockchain_destroy(blockchain_t *blockchain);
uint8_t *block_hash(block_t const *block, uint8_t hash_buf[SHA256_DIGEST_LENGTH]);
int blockchain_serialize(blockchain_t const *blockchain, char const *path);
void serialize_blocks(int fd, int encoding, llist_t *list);
void write_attr(int fd, int encoding, void *attr, size_t size);
int read_attr(int fd, int encoding, void *attr, size_t size);
blockchain_t *blockchain_deserialize(char const *path);
int block_is_valid(block_t const *block, block_t const *prev_block);

/* Block mining functions */
int hash_matches_difficulty(uint8_t const hash[SHA256_DIGEST_LENGTH], uint32_t difficulty);
void block_mine(block_t *block);
uint32_t blockchain_difficulty(blockchain_t const *blockchain);

#endif /* _BLOCKCHAIN_ */
