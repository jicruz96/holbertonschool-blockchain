#include <hblk_crypto.h>
#include <string.h>
#include <stdio.h>

/**
 * ec_to_pub - extracts the public key from an EC_KEY opaque structure
 * @key: pointer to EC_KEY structure to retrieve the public key from.
 * @pub: address where extracted public key is stored, uncompressed
 * Return: pointer to pub or NULL if key is NULL or failure occurs
 **/
uint8_t *ec_to_pub(EC_KEY const *key, uint8_t pub[EC_PUB_LEN])
{
	unsigned char *tmp;
	size_t key_len;

	if (!key || !pub)
		return (NULL);

	key_len = EC_KEY_key2buf(key, POINT_CONVERSION_UNCOMPRESSED, &tmp, NULL);
	if (!key_len)
		return (NULL);

	memcpy(pub, tmp, key_len);
	free(tmp);
	return (pub);
}
