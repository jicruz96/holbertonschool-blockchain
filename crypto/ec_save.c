#include <hblk_crypto.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <openssl/pem.h>
#include <string.h>


FILE *open_file(char const *folder, char const *file);


/**
 * ec_save - saves an existing EC key pair on the disk.
 * @key: points to the EC key pair to be saved on disk
 * @folder: path to folder in which to save the keys (e.g. /home/hblk/alex)
 * Return: 1 or 0 upon success or failure
 */
int ec_save(EC_KEY *key, char const *folder)
{
	FILE *pub = NULL, *priv = NULL;
	struct stat st = {0};
	int status;


	if (
		(stat(folder, &st) == -1 && mkdir(folder, 0700) == -1) ||
		!(pub  = open_file(folder, PUB_FILENAME))              ||
		!(priv = open_file(folder, PRI_FILENAME))              ||
		!PEM_write_EC_PUBKEY(pub, key)                         ||
		!PEM_write_ECPrivateKey(priv, key, NULL, NULL, 0, NULL, NULL)
	)
		status = 0;
	else
		status = 1;

	if (pub)  fclose(pub);
	if (priv) fclose(priv);
	return (status);
}

/**
 * open_file - given a file and folder, open file and return reference to it
 * @folder: file's parent folder
 * @filename: name of file to open
 * Return: pointer to opened file or NULL on failure
 **/
FILE *open_file(char const *folder, char const *filename)
{
	size_t size = sizeof(char) * (strlen(folder) + 1 + strlen(filename) + 1);
	char *filepath = malloc(size);
	FILE *file;


	if (filepath)
	{
		sprintf(filepath, "%s/%s", folder, filename);
		file = fopen(filepath, "w");
		free(filepath);
		return (file);
	}

	return (NULL);
}
