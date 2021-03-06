#ifndef _HEADER_
#define _HEADER_


#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <blockchain.h>
#define default_prompt ">>> "
#define default_hblk_file "save.hblk"
#define KEEP_PREVIOUS_STATUS INT_MIN
#define is_numeric(x) ((x) >= '0' && (x) <= '9')
#define is_lowercase_alpha(x) ((x) >= 'a' && (x) <= 'z')

/* GLOBALS */
char *program_name;
int running;
llist_t *wallets;
EC_KEY *current_wallet;
blockchain_t *blockchain;
llist_t *transaction_pool;
size_t line_size;
char *line;
block_t *prev_block;

/**
 * struct blockchain_cli_command_s - associates a cli thingie command with a function
 * @command: command (string)
 * @func: associated function that performs command
 **/
typedef struct blockchain_cli_command_s
{
	char *command;
	int (*func)(char **args);
} blockchain_cli_command_t;


#endif /* _HEADER_ */
