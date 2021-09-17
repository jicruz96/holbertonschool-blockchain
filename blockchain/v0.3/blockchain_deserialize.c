#include <blockchain.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>

static void bswap(uint8_t *p, size_t size);
static int read_attr(int fd, int encoding, void *attr, size_t size);
static llist_t *read_transactions(int fd, int encoding);
static llist_t *read_inputs(int fd, int encoding, int num_inputs);
static llist_t *read_outputs(int fd, int encoding, int num_outputs);

/**
 * read_transactions - reads transactions from a serialized blockchain file
 * @fd: file descriptor
 * @encoding: file encoding
 * Return: list of transactions
 **/
static llist_t *read_transactions(int fd, int encoding)
{
	uint32_t num_txs, num_inputs, num_outputs;
	transaction_t *tx;
	llist_t *list;

	if (!read_attr(fd, encoding, &num_txs, sizeof(num_txs)) || !(list = llist_create(MT_SUPPORT_FALSE)))
		return (NULL);
	if ((int)num_txs <= 0)
	{
		llist_destroy(list, false, NULL);
		return (NULL);
	}

	while ((int)num_txs-- > 0)
	{
		tx = calloc(1, sizeof(transaction_t));
		llist_add_node(list, tx, ADD_NODE_REAR);
		if (
			!read_attr(fd, encoding, &tx->id, sizeof(tx->id)) ||
			!read_attr(fd, encoding, &num_inputs, sizeof(num_inputs)) ||
			!read_attr(fd, encoding, &num_outputs, sizeof(num_outputs)) ||
			!(tx->inputs = read_inputs(fd, encoding, num_inputs)) ||
			!(tx->outputs = read_outputs(fd, encoding, num_outputs))
		)
		{
			llist_destroy(list, true, (node_dtor_t)transaction_destroy);
			return (NULL);
		}
	}

	return (list);
}

/**
 * read_inputs - reads transaction inputs from a serialized blockchain file
 * @fd: file descriptor
 * @encoding: encoding (MSB or LSB)
 * @num_inputs: number of anticipated inputs
 * Return: pointer to list of transaction inputs or NULL on failure
 **/
static llist_t *read_inputs(int fd, int encoding, int num_inputs)
{
	llist_t *inputs;
	tx_in_t *in;

	if (num_inputs <= 0 || !(inputs = llist_create(MT_SUPPORT_FALSE)))
		return (NULL);

	while ((int)num_inputs-- > 0)
	{

		in = calloc(1, sizeof(tx_in_t));
		llist_add_node(inputs, in, ADD_NODE_REAR);
		if (
			!read_attr(fd, encoding, &in->block_hash, sizeof(in->block_hash)) ||
			!read_attr(fd, encoding, &in->tx_id, sizeof(in->tx_id)) ||
			!read_attr(fd, encoding, &in->tx_out_hash, sizeof(in->tx_out_hash)) ||
			!read_attr(fd, encoding, &in->sig.sig, sizeof(in->sig.sig)) ||
			!read_attr(fd, encoding, &in->sig.len, sizeof(in->sig.len))
		)
		{
			llist_destroy(inputs, true, NULL);
			return (NULL);
		}
	}
	return (inputs);
}

/**
 * read_outputs - reads transaction outputs from a serialized blockchain file
 * @fd: file descriptor
 * @encoding: encoding (MSB or LSB)
 * @num_outputs: number of anticipated outputs
 * Return: pointer to list of transaction outputs or NULL on failure
 **/
static llist_t *read_outputs(int fd, int encoding, int num_outputs)
{
	llist_t *outputs;
	tx_out_t *out;

	if (num_outputs <= 0 || !(outputs = llist_create(MT_SUPPORT_FALSE)))
		return (NULL);

	while ((int)num_outputs-- > 0)
	{
		out = calloc(1, sizeof(tx_out_t));
		llist_add_node(outputs, out, ADD_NODE_REAR);
		if (
			!read_attr(fd, encoding, &out->amount, sizeof(out->amount)) ||
			!read_attr(fd, encoding, &out->pub, sizeof(out->pub)) ||
			!read_attr(fd, encoding, &out->hash, sizeof(out->hash))
		)
		{
			llist_destroy(outputs, true, NULL);
			return (NULL);
		}
	}
	return (outputs);
}

/**
 * blockchain_deserialize - deserializes a Blockchain from a file
 * @path: contains the path to a file to load the Blockchain from
 *     * return NULL if file doesn’t exist or we don't permission to open
 * Return: pointer to deserialized Blockchain, or NULL upon failure
 **/
blockchain_t *blockchain_deserialize(char const *path)
{
	int fd = open(path, O_RDONLY);
	unsigned char buf[sizeof(block_t)];
	uint8_t encoding;
	uint32_t num_blocks, num_unspent;
	blockchain_t *blockchain;
	block_t *block;
	unspent_tx_out_t *utxo;

	if (fd == -1)
		return (NULL);

	if (
		read(fd, buf, sizeof(HBLK_MAGIC) - 1) < (ssize_t)sizeof(HBLK_MAGIC) - 1   ||
		memcmp(buf, HBLK_MAGIC, sizeof(HBLK_MAGIC) - 1)                  ||
		read(fd, buf, sizeof(HBLK_VERSION) - 1) < (ssize_t)sizeof(HBLK_VERSION) - 1 ||
		!(blockchain = blockchain_create())
	)
	{
		close(fd);
		return (NULL);
	}

	block_destroy((block_t *)llist_pop(blockchain->chain));
	read(fd, &encoding, sizeof(encoding));
	read_attr(fd, encoding, &num_blocks, sizeof(num_blocks));
	read_attr(fd, encoding, &num_unspent, sizeof(num_unspent));
	while ((int)num_blocks-- > 0)
	{
		block = calloc(1, sizeof(block_t));
		if (
			!read_attr(fd, encoding, &block->info.index,      sizeof(block->info.index))      ||
			!read_attr(fd, encoding, &block->info.difficulty, sizeof(block->info.difficulty)) ||
			!read_attr(fd, encoding, &block->info.timestamp,  sizeof(block->info.timestamp))  ||
			!read_attr(fd, encoding, &block->info.nonce,      sizeof(block->info.nonce))      ||
			!read_attr(fd, encoding,  block->info.prev_hash,  sizeof(block->info.prev_hash))  ||
			!read_attr(fd, encoding, &block->data.len,        sizeof(block->data.len))        ||
			!read_attr(fd, encoding,  block->data.buffer,     block->data.len)                ||
			!read_attr(fd, encoding,  block->hash,            sizeof(block->hash))
		)
		{
			llist_destroy(blockchain->chain, true, (node_dtor_t)block_destroy);
			free(blockchain);
			close(fd);
			return (NULL);
		}
		block->transactions = read_transactions(fd, encoding);
		llist_add_node(blockchain->chain, block, ADD_NODE_REAR);
	}
	while ((int)num_unspent-- > 0)
	{
		utxo = calloc(1, sizeof(unspent_tx_out_t));
		if (
			!read_attr(fd, encoding, &utxo->block_hash, sizeof(utxo->block_hash)) ||
			!read_attr(fd, encoding, &utxo->tx_id, sizeof(utxo->tx_id)) ||
			!read_attr(fd, encoding, &utxo->out.amount, sizeof(utxo->out.amount)) ||
			!read_attr(fd, encoding, &utxo->out.pub, sizeof(utxo->out.pub)) ||
			!read_attr(fd, encoding, &utxo->out.hash, sizeof(utxo->out.hash))
		)
		{
			llist_destroy(blockchain->chain, true, (node_dtor_t)block_destroy);
			llist_destroy(blockchain->unspent, true, NULL);
			free(blockchain);
			close(fd);
			return (NULL);
		}
		llist_add_node(blockchain->unspent, utxo, ADD_NODE_REAR);
	}
	close(fd);
	return (blockchain);
}

/**
 * read_attr - reads an ELF file attribute from an ELF file
 * @fd: file descriptor
 * @encoding: encoding of attribute
 * @attr: pointer to attribute where value will be stored
 * @size: size of attribute
 * Return: 1 on success | 0 on failure
 **/
static int read_attr(int fd, int encoding, void *attr, size_t size)
{
	static int bytes_read;
	ssize_t res;

	res = read(fd, attr, size);
	bytes_read += res;
	if (res == -1)
		perror(NULL);
	if (res != (ssize_t)size)
		return (0);

	if (encoding == MSB)
		bswap((uint8_t *)attr, size);

	return (1);
}

/**
 * bswap - byte swap
 * @p: pointer to data
 * @size: data size
 **/
static void bswap(uint8_t *p, size_t size)
{
	uint8_t buf[64] = {0};
	int i;

	for (i = size - 1; i >= 0; i--)
		buf[size - i - 1] = p[i];

	for (i = 0; i < (int)size; i++)
		p[i] = buf[i];
}
