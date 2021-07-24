#include <hblk_crypto.h>
#include <openssl/pem.h>
#include <string.h>

static FILE *open_file(char const *folder, char const *filename, char *mode);

/**
 * ec_load - loads an EC key pair from the disk.
 * @folder: path to folder from which to load the keys
 * Return: pointer to the created EC key pair on success, or NULL on failure
 **/
EC_KEY *ec_load(char const *folder)
{
	FILE *pubfile, *privfile;
	EC_KEY *key;

	if (!(pubfile  = open_file(folder, PUB_FILENAME, "r")))
		return (NULL);

	if (!(privfile = open_file(folder, PRI_FILENAME, "r")))
	{
		fclose(pubfile);
		return (NULL);
	}

	if (
		!(key = EC_KEY_new_by_curve_name(EC_CURVE))    ||
		!PEM_read_EC_PUBKEY(pubfile, &key, NULL, NULL) ||
		!PEM_read_ECPrivateKey(privfile, &key, NULL, NULL)
	)
	{
		EC_KEY_free(key);
		key = NULL;
	}

	fclose(pubfile);
	fclose(privfile);
	return (key);
}

/**
 * open_file - given a file and folder, open file and return reference to it
 * @folder: file's parent folder
 * @filename: name of file to open
 * @mode: mode string to pass to fopen
 * Return: pointer to opened file or NULL on failure
 **/
static FILE *open_file(char const *folder, char const *filename, char *mode)
{
	char *filepath;
	FILE *file;

	if (!folder || !filename)
		return (NULL);

	filepath = malloc(sizeof(char) * (strlen(folder) + strlen(filename) + 2));
	if (!filepath)
		return (NULL);

	sprintf(filepath, "%s/%s", folder, filename);
	file = fopen(filepath, mode);
	free(filepath);
	return (file);
}
