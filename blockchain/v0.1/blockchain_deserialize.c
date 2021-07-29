#include <blockchain.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>

static void bswap(uint8_t *p, size_t size);
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
	uint32_t num_blocks;
	blockchain_t *blockchain;
	block_t *block;

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
	while (num_blocks--)
	{
		block = malloc(sizeof(block_t));
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
		llist_add_node(blockchain->chain, block, ADD_NODE_REAR);
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
int read_attr(int fd, int encoding, void *attr, size_t size)
{
	if (read(fd, attr, size) != (ssize_t)size)
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
