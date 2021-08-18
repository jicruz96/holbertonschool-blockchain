#include <blockchain.h>

#define BYTE_SIZE 8
/**
 * hash_matches_difficulty - checks if a given hash matches a given difficulty
 * @hash: hash to check
 * @difficulty: minimum difficulty the hash should match
 * Return: 1 if the difficulty is respected, or 0 otherwise
 **/
int hash_matches_difficulty(uint8_t const hash[SHA256_DIGEST_LENGTH], uint32_t difficulty)
{
	size_t i;

	if (!difficulty)
		return (1);

	for (i = 0; difficulty >= BYTE_SIZE; difficulty -= BYTE_SIZE, i++)
		if (hash[i] != 0)
			return (0);

	return ((hash[i] >> (BYTE_SIZE - difficulty)) == 0);
}
