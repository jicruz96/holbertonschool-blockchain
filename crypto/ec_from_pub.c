#include <hblk_crypto.h>
#include <openssl/ossl_typ.h>

/**
 * ec_from_pub - creates an EC_KEY structure given a public key
 * @pub: contains the public key to be converted
 * Return: pointer to created EC_KEY structure on success, NULL on failure
 **/
EC_KEY *ec_from_pub(uint8_t const pub[EC_PUB_LEN])
{
	EC_KEY *key = NULL;
	EC_POINT *point = NULL;
	const EC_GROUP *group;

	if (
		!pub                                                        ||
		!(key   = EC_KEY_new_by_curve_name(EC_CURVE))               ||
		!(group = EC_KEY_get0_group(key))                           ||
		!(point = EC_POINT_new(group))                              ||
		!(EC_POINT_oct2point(group, point, pub, EC_PUB_LEN, NULL))  ||
		!(EC_KEY_set_public_key(key, point))
	)
	{
		EC_KEY_free(key);
		key = NULL;
	}

	EC_POINT_free(point);
	return (key);

}
