#include <hblk_crypto.h>
#include <stdio.h>

/**
 * sha256 - returns the sha256 hash for the string s
 * @s: string of bytes
 * @len: number of bytes in s
 * @digest: where resulting hash is stored
 * Return: pointer to digest
 **/
uint8_t
*sha256(int8_t const *s, size_t len, uint8_t digest[SHA256_DIGEST_LENGTH])
{
	if (!digest)
		return (NULL);

	return (SHA256((const unsigned char *)s, len, (unsigned char *)digest));
}
