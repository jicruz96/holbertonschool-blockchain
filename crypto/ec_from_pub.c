#include <hblk_crypto.h>

/**
 * ec_from_pub - creates an EC_KEY structure given a public key
 * @pub: contains the public key to be converted
 * Return: pointer to created EC_KEY structure on success, NULL on failure
 **/
EC_KEY *ec_from_pub(uint8_t const pub[EC_PUB_LEN])
{
	EC_KEY *key;

	if (!pub)
		return (NULL);

	key = EC_KEY_new_by_curve_name(EC_CURVE);

	if (!key || !EC_KEY_oct2key(key, pub, EC_PUB_LEN, NULL))
	{
		EC_KEY_free(key);
		return (NULL);
	}

	return (key);

}
