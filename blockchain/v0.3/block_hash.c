#include <blockchain.h>
#include <string.h>
#include <stdio.h>

/**
 * block_hash - computes the hash of a Block
 * @block: pointer to block to be hashed
 * @hash_buf: where hash will be stored
 * Return: pointer to hash_buf
 **/
uint8_t
*block_hash(block_t const *block, uint8_t hash_buf[SHA256_DIGEST_LENGTH])
{
	size_t len;
	uint8_t *buf;
	int i, size;
	transaction_t *tmp_tx;

	if (!block || !hash_buf)
		return (NULL);

	if (block->transactions)
		size = llist_size(block->transactions);
	else
		size = 0;
	len = sizeof(block->info) + block->data.len;
	buf = calloc(1, len + (size * sizeof(transaction_t)));
	memcpy(buf, block, len);

	for (i = 0 ; i < size; i++)
	{
		tmp_tx = llist_get_node_at(block->transactions, i);
		memcpy(buf + len, tmp_tx, sizeof(*tmp_tx));
		len += sizeof(*tmp_tx);
	}

	if (!sha256((int8_t const *)buf, len, hash_buf))
	{
		free(buf);
		return (NULL);
	}

	free(buf);
	return (hash_buf);
}
