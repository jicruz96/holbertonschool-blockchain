#include <blockchain.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>

static void bswap(uint8_t *p, size_t size);
static void write_attr(int fd, int encoding, void *attr, size_t size);
static void serialize_blocks(int fd, int encoding, llist_t *list);
static void serialize_unspent(int fd, int encoding, llist_t *list);
static void serialize_transactions(int fd, int encoding, llist_t *list);
static void serialize_transaction_inputs(int fd, int encoding, llist_t *list);
static void serialize_transaction_outputs(int fd, int encoding, llist_t *list);

/**
 * blockchain_serialize - serializes a Blockchain into a file
 * @blockchain: points to the Blockchain to be serialized,
 * @path: contains the path to a file to serialize the Blockchain into
 *            * If path points to an existing file, it must be overwritten
 * Return: 0 upon success, or -1 upon failure
 **/
int blockchain_serialize(blockchain_t const *blockchain, char const *path)
{
	int fd;
	uint32_t num_blocks, num_unspent;
	uint8_t encoding = HBLK_ENDIAN;

	if (!blockchain || !path)
		return (-1);

	fd = open(path, O_WRONLY | O_CREAT, 0644);
	if (fd == -1)
		return (-1);

	num_blocks = llist_size(blockchain->chain);
	num_unspent = llist_size(blockchain->unspent);

	write(fd, HBLK_MAGIC,   sizeof(HBLK_MAGIC)   - 1);
	write(fd, HBLK_VERSION, sizeof(HBLK_VERSION) - 1);
	write(fd, &encoding,    sizeof(encoding));
	write(fd, &num_blocks,  sizeof(num_blocks));
	write(fd, &num_unspent, sizeof(num_unspent));
	serialize_blocks(fd, HBLK_ENDIAN, blockchain->chain);
	serialize_unspent(fd, HBLK_ENDIAN, blockchain->unspent);
	close(fd);
	return (0);
}
/**
 * serialize_blocks - serializes blocks
 * @fd: file descriptor
 * @encoding: file encoding
 * @list: list of blocks
 **/
static void serialize_blocks(int fd, int encoding, llist_t *list)
{
	block_t *block;
	int i, num_blocks = llist_size(list), num_txs;

	for (i = 0; i < num_blocks; i++)
	{
		block = llist_get_node_at(list, i);
		num_txs = llist_size(block->transactions);
		if (num_txs == -1)
			num_txs = 0;
		write_attr(fd, encoding, &block->info.index,      sizeof(block->info.index));
		write_attr(fd, encoding, &block->info.difficulty, sizeof(block->info.difficulty));
		write_attr(fd, encoding, &block->info.timestamp,  sizeof(block->info.timestamp));
		write_attr(fd, encoding, &block->info.nonce,      sizeof(block->info.nonce));
		write_attr(fd, encoding, block->info.prev_hash,   sizeof(block->info.prev_hash));
		write_attr(fd, encoding, &block->data.len,        sizeof(block->data.len));
		write_attr(fd, encoding, block->data.buffer,      block->data.len);
		write_attr(fd, encoding, block->hash,             sizeof(block->hash));
		write_attr(fd, encoding, &num_txs,                sizeof(num_txs));
		serialize_transactions(fd, encoding, block->transactions);
	}

}

/**
 * serialize_unspent - serializes unspent transaction outputs
 * @fd: file descriptor
 * @encoding: file encoding
 * @list: list of unspent transaction outputs
 **/
static void serialize_unspent(int fd, int encoding, llist_t *list)
{
	unspent_tx_out_t *utxo;
	int i, num_unspent = llist_size(list);

	for (i = 0; i < num_unspent; i++)
	{
		utxo = llist_get_node_at(list, i);
		write_attr(fd, encoding, &utxo->block_hash, sizeof(utxo->block_hash));
		write_attr(fd, encoding, &utxo->tx_id, sizeof(utxo->tx_id));
		write_attr(fd, encoding, &utxo->out.amount, sizeof(utxo->out.amount));
		write_attr(fd, encoding, &utxo->out.pub, sizeof(utxo->out.pub));
		write_attr(fd, encoding, &utxo->out.hash, sizeof(utxo->out.hash));
	}
}

/**
 * serialize_transactions - serializes transactions
 * @fd: file descriptor
 * @encoding: file encoding
 * @list: list of transactions
 **/
static void serialize_transactions(int fd, int encoding, llist_t *list)
{
	transaction_t *tx;
	int i, num_txs, num_inputs, num_outputs;

	num_txs = llist_size(list);
	for (i = 0; i < num_txs; i++)
	{
		tx = llist_get_node_at(list, i);
		num_inputs = llist_size(tx->inputs);
		num_outputs = llist_size(tx->outputs);
		write_attr(fd, encoding, &tx->id, sizeof(tx->id));
		write_attr(fd, encoding, &num_inputs, sizeof(num_inputs));
		write_attr(fd, encoding, &num_outputs, sizeof(num_outputs));
		serialize_transaction_inputs(fd, encoding, tx->inputs);
		serialize_transaction_outputs(fd, encoding, tx->outputs);
	}
}

/**
 * serialize_transaction_inputs - serializes transaction inputs
 * @fd: file descriptor
 * @encoding: file encoding
 * @list: list of transactions
 **/
static void serialize_transaction_inputs(int fd, int encoding, llist_t *list)
{
	tx_in_t *in;
	int i, num_inputs;

	num_inputs = llist_size(list);
	for (i = 0; i < num_inputs; i++)
	{
		in = llist_get_node_at(list, i);
		write_attr(fd, encoding, &in->block_hash, sizeof(in->block_hash));
		write_attr(fd, encoding, &in->tx_id, sizeof(in->tx_id));
		write_attr(fd, encoding, &in->tx_out_hash, sizeof(in->tx_out_hash));
		write_attr(fd, encoding, &in->sig.sig, sizeof(in->sig.sig));
		write_attr(fd, encoding, &in->sig.len, sizeof(in->sig.len));
	}
}

/**
 * serialize_transaction_outputs - serializes transaction outputs
 * @fd: file descriptor
 * @encoding: file encoding
 * @list: list of transactions
 **/
static void serialize_transaction_outputs(int fd, int encoding, llist_t *list)
{
	tx_out_t *out;
	int i, num_outputs;

	num_outputs = llist_size(list);
	for (i = 0; i < num_outputs; i++)
	{
		out = llist_get_node_at(list, i);
		write_attr(fd, encoding, &out->amount, sizeof(out->amount));
		write_attr(fd, encoding, &out->pub, sizeof(out->pub));
		write_attr(fd, encoding, &out->hash, sizeof(out->hash));
	}
}


/**
 * write_attr - writes an attribute into a file in the proper encoding
 * @fd: file descriptor
 * @encoding: file encoding
 * @attr: attribute to write
 * @size: size of attribute
 **/
void write_attr(int fd, int encoding, void *attr, size_t size)
{
	if (encoding == MSB)
		bswap((uint8_t *)attr, size);

	write(fd, attr, size);
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
