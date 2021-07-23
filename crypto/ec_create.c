#include <hblk_crypto.h>

/**
 * ec_create - creates a new EC key pair
 * Return: pointer to an EC_KEY structure, containing both the public and
 *         private keys, or NULL upon failure
 **/
EC_KEY *ec_create(void)
{
	EC_KEY *key = EC_KEY_new_by_curve_name(EC_CURVE);

	if (key && !EC_KEY_generate_key(key))
	{
		EC_KEY_free(key);
		return (NULL);
	}
	return (key);
}
