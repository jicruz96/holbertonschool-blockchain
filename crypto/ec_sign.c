#include <hblk_crypto.h>

/**
 * ec_sign - signs a given set of bytes, using a given EC_KEY private key
 * @key: EC_KEY structure containing private key used for signature
 * @msg: characters to be signed
 * @msglen: number of characters
 * @sig: address at which to store the signature
 * Return: pointer to signature buffer on success (sig->sig) | NULL on failure
 **/
uint8_t
*ec_sign(EC_KEY const *key, uint8_t const *msg, size_t msglen, sig_t *sig)
{
	EC_KEY *tmp;

	if (
		!key    ||
		!msg    ||
		!msglen ||
		!sig    ||
		!(tmp = EC_KEY_dup(key)) ||
		!ECDSA_sign(0, msg, msglen, sig->sig, (unsigned int *)&sig->len, tmp)
	)
		return (NULL);

	EC_KEY_free(tmp);
	return (sig->sig);

}
