#include <hblk_crypto.h>

/**
 * ec_verify - verifies signature of a set of bytes, using a given public key
 * @key: EC_KEY structure containing public key used to verify the signature
 * @msg: characters to verify the signature of
 * @msglen: number of characters
 * @sig: signature to be checked
 * Return: 1 if the signature is valid, and 0 otherwise
 */
int ec_verify(EC_KEY const *key, uint8_t const *msg,
	size_t msglen, sig_t const *sig)
{
	EC_KEY *tmp;
	int status;

	if (
		!key    ||
		!msg    ||
		!msglen ||
		!sig    ||
		!(tmp = EC_KEY_dup(key)) ||
		!ECDSA_verify(0, msg, msglen, sig->sig, sig->len, tmp)
	)
		status = 0;
	else
		status = 1;

	EC_KEY_free(tmp);
	return (status);
}
