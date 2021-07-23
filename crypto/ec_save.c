#include <hblk_crypto.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <openssl/pem.h>
#include <string.h>

/**
 * ec_save - saves an existing EC key pair on the disk.
 * @key: points to the EC key pair to be saved on disk
 * @folder: path to folder in which to save the keys (e.g. /home/hblk/alex)
 * Return: 1 or 0 upon success or failure
 */
int ec_save(EC_KEY *key, char const *folder)
{
	struct stat st = {0};
	char *pubkey_file, *privkey_file;
	FILE *pub, *priv;
	int status;

	if (stat(folder, &st) == -1 && mkdir(folder, 0700) == -1)
		return (0);

	pubkey_file = malloc(sizeof(char) * (strlen(folder) + sizeof(PUB_FILENAME)));
	privkey_file = malloc(sizeof(char) * (strlen(folder) + sizeof(PRI_FILENAME)));

	sprintf(pubkey_file, "%s/%s", folder, PUB_FILENAME);
	sprintf(pubkey_file, "%s/%s", folder, PRI_FILENAME);

	pub = fopen(pubkey_file, "w");
	priv = fopen(privkey_file, "w");

	if (
		!PEM_write_EC_PUBKEY(pub, key) ||
		!PEM_write_ECPrivateKey(priv, key, NULL, NULL, 0, NULL, NULL)
	)
		status = 0;
	else
		status = 1;
	fclose(pub);
	fclose(priv);
	free(pubkey_file);
	free(privkey_file);
	return (status);
}
