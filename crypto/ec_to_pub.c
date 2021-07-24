#include <hblk_crypto.h>

#define UNCOMPRESSED POINT_CONVERSION_UNCOMPRESSED

/**
 * ec_to_pub - extracts the public key from an EC_KEY opaque structure
 * @key: pointer to EC_KEY structure to retrieve the public key from.
 * @pub: address where extracted public key is stored, uncompressed
 * Return: pointer to pub or NULL if key is NULL or failure occurs
 **/
uint8_t *ec_to_pub(EC_KEY const *key, uint8_t pub[EC_PUB_LEN])
{
	const EC_POINT *point;
	const EC_GROUP *group;


	if (
		!key ||
		!pub ||
		!(point = EC_KEY_get0_public_key(key)) ||
		!(group = EC_KEY_get0_group(key)) ||
		!EC_POINT_point2oct(group, point, UNCOMPRESSED, pub, EC_PUB_LEN, NULL)
		)
		return (NULL);
	return (pub);
}
