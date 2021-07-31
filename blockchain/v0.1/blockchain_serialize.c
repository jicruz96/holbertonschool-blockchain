#include <blockchain.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>

static void bswap(uint8_t *p, size_t size);

/**
 * blockchain_serialize - serializes a Blockchain into a file
 * @blockchain: points to the Blockchain to be serialized,
 * @path: contains the path to a file to serialize the Blockchain into
 *            * If path points to an existing file, it must be overwritten
 * Return: 0 upon success, or -1 upon failure
 **/
int blockchain_serialize(blockchain_t const *blockchain, char const *path)
{
	int fd, num_blocks;
	uint8_t encoding = HBLK_ENDIAN;

	if (!blockchain || !path)
		return (-1);

	fd = open(path, O_WRONLY | O_CREAT, 0644);
	if (fd == -1)
		return (-1);
	num_blocks = llist_size(blockchain->chain);
	write(fd, HBLK_MAGIC,   sizeof(HBLK_MAGIC)   - 1);
	write(fd, HBLK_VERSION, sizeof(HBLK_VERSION) - 1);
	write(fd, &encoding,  sizeof(encoding));
	write(fd, &num_blocks,  sizeof(num_blocks));
	serialize_blocks(fd, HBLK_ENDIAN, blockchain->chain);
	close(fd);
	return (0);
}
/**
 * serialize_blocks - serializes blocks
 * @fd: file descriptor
 * @encoding: file encoding
 * @list: list of blocks
 **/
void serialize_blocks(int fd, int encoding, llist_t *list)
{
	block_t *block;
	int i, num_blocks = llist_size(list);

	for (i = 0; i < num_blocks; i++)
	{
		block = llist_get_node_at(list, i);
		write_attr(fd, encoding, &block->info.index,      sizeof(block->info.index));
		write_attr(fd, encoding, &block->info.difficulty, sizeof(block->info.difficulty));
		write_attr(fd, encoding, &block->info.timestamp,  sizeof(block->info.timestamp));
		write_attr(fd, encoding, &block->info.nonce,      sizeof(block->info.nonce));
		write_attr(fd, encoding, block->info.prev_hash,   sizeof(block->info.prev_hash));
		write_attr(fd, encoding, &block->data.len,        sizeof(block->data.len));
		write_attr(fd, encoding, block->data.buffer,      block->data.len);
		write_attr(fd, encoding, block->hash,             sizeof(block->hash));
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
