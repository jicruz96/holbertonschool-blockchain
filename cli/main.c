#include "header.h"

/**
 * print_error - function
 * @msg: message
 **/
void print_error(char *msg)
{
	char error_msg[64];

	sprintf(error_msg, "%s: %s", program_name, msg);
	perror(error_msg);
}
/**
 * error_out - function
 * @msg: message
 **/
int error_out(char *msg)
{
	print_error(msg);
	running = 0;
	return (errno);
}

/**
 * startup - startup globals
 */
__attribute__((constructor))int startup(void)
{
	wallets = llist_create(MT_SUPPORT_FALSE);
	if (!wallets)
		return (error_out("wallets list failed to initialize"));
	transaction_pool = llist_create(MT_SUPPORT_FALSE);
	if (!transaction_pool)
		return (error_out("local transaction pool failed to initialize"));
	line_size = 0;
	line = NULL;
	running = 1;
	current_wallet = NULL;
	blockchain = NULL;
	return (0);
}

/**
 * cleanup - clean up
 */
__attribute__((destructor))void cleanup(void)
{
	llist_destroy(wallets, 1, (node_dtor_t)EC_KEY_free);
	blockchain_destroy(blockchain);
	free(line);
}

/**
 * wallet_load - wallet_load function
 * @args: command arguments
 * Return: status
 **/
int wallet_load(char **args)
{
	char *folder = args[1] ? args[1] : ".";
	EC_KEY *wallet = ec_load(folder);

	if (!wallet)
	{
		printf("No wallet in folder [%s]\n", folder);
WALLET_LOAD_CREATE_KEY_CONFIRM:
		printf("Create new key? [y/n]: ");
		getline(&line, &line_size, stdin);

		if (*line != 'y' && *line != 'n')
		{
			printf("Invalid input\n");
			goto WALLET_LOAD_CREATE_KEY_CONFIRM;
		}

		if (*line == 'n')
		{
			printf("Aborting.\n");
			return (-1);
		}

		printf("Creating new wallet\n");
		wallet = ec_create();

		if (!wallet)
		{
			print_error("wallet_load: failed to create wallet");
			return (errno);
		}

		if (ec_save(wallet, folder) == -1)
		{
			print_error("wallet_load: failed to save wallet");
			return (errno);
		}
	}

	if (llist_add_node(wallets, wallet, ADD_NODE_REAR) == -1)
	{
		print_error("wallet_load: failed to add to wallets list");
		EC_KEY_free(wallet);
		return (errno);
	}

	printf("Wallet loaded");

	if (strcmp(folder, "."))
		printf(" from %s\n", folder);
	else
		printf("\n");

	current_wallet = wallet;

	return (0);
}

/**
 * wallet_save - wallet_save function
 * @args: command arguments
 * Return: status
 **/
int wallet_save(char **args)
{
	char *folder = args[1] ? args[1] : ".";

	if (!current_wallet)
	{
		fprintf(stderr, "ERROR: No wallet is currently loaded.\n");
		fprintf(stderr, "Use `wallet_load` to create a new wallet.\n");
		return (-1);
	}

	if (ec_save(current_wallet, folder) == -1)
	{
		print_error("wallet_save: failed to save wallet");
		return (errno);
	}

	if (!args[1])
		printf("Wallet saved in current folder.\n");
	else
		printf("Wallet saved in folder [%s]\n", folder);

	return (0);
}
/**
 * get_amount - gets amount
 * @s: string
 * Return: amount
 **/
uint32_t get_amount(char *s)
{
	int i;

	if (!s)
		return (0);

	for (i = 0; s[i]; i++)
		if (s[i] < '0' || s[i] > '9')
			return (0);

	return ((uint32_t)strtoul(s, NULL, 10));
}

/**
 * EC_KEY - function
 * @s: string representation of pub key
 * Return: status
 **/
const EC_KEY *get_key(char *s)
{
	int i;
	uint8_t tmp, pub[EC_PUB_LEN];
	char buf[3];


	if (!s)
		return (NULL);

	for (i = 0; i < 2 * EC_PUB_LEN; i++)
		if (!is_numeric(s[i]) && !is_lowercase_alpha(s[i]))
			return (NULL);

	if (s[i] != '\0')
		return (NULL);

	for (i = 0; i < EC_PUB_LEN; i++) /* interpret hex string */
	{
		memcpy(buf, s + (2 * i), 2);
		tmp = strtoul(buf, NULL, 16);
		memcpy(pub + i, &tmp, sizeof(uint8_t));
	}

	return (ec_from_pub(pub));
}

/**
 * send - send function
 * @args: command arguments
 * Return: status
 **/
int send(char **args)
{
	uint32_t amount;
	const EC_KEY *address;
	transaction_t *transaction;

	if (!blockchain)
	{
		fprintf(stderr, "ERROR: No blockchain loaded.\n");
		fprintf(stderr, "Use command `load` to load a blockchain.\n");
		return (-1);
	}

	if (!args[1] || !args[2])
	{
		fprintf(stderr, "Usage: send <amount> <address>\n");
		return (-1);
	}

	if (!(amount = get_amount(args[1])))
	{
		fprintf(stderr, "Amount must be positive integer\n");
		return (-1);
	}

	if (!(address = get_key(args[2])))
	{
		fprintf(stderr, "ERROR: Invalid wallet address\n");
		return (-1);
	}

	if (
		!(transaction = transaction_create(
								current_wallet,
								address,
								amount,
								blockchain->unspent
								))
	)
	{
		print_error("send: could not create transaction");
		return (errno);
	}

	if (!transaction_is_valid(transaction, blockchain->unspent))
	{
		print_error("send: could not validate transaction");
		transaction_destroy(transaction);
		return (-1);
	}

	if (llist_add_node(transaction_pool, transaction, ADD_NODE_REAR) == -1)
	{
		print_error("send: could not add transaction to transaction pool");
		transaction_destroy(transaction);
		return (errno);
	}

	printf("Transaction added to transaction pool\n");
	return (0);
}
/**
 * mine - mine function
 * @args: command arguments
 * Return: status
 **/
int mine(char **args)
{
	(void)args;
	printf("This is %s\n", args[0]);
	return (0);
}
/**
 * info - info function
 * @args: command arguments
 * Return: status
 **/
int info(char **args)
{
	(void)args;
	printf("This is %s\n", args[0]);
	return (0);
}
/**
 * load - load function
 * @args: command arguments
 * Return: status
 **/
int load(char **args)
{
	blockchain_t *tmp = blockchain_deserialize(args[1]);
	int i, num_blocks;
	block_t *prev_block = NULL, *block;

	if (!tmp)
	{
		print_error("load");
		return (errno);
	}
	num_blocks = llist_size(tmp->chain);
	for (i = 0; i < num_blocks; i++)
	{
		block = llist_get_node_at(tmp->chain, i);
		if (!block_is_valid(block, prev_block, tmp->unspent))
		{
			print_error("load: Invalid blockchain");
			blockchain_destroy(tmp);
			return (-1);
		}
		prev_block = block;
	}
	blockchain = tmp;
	return (0);
}
/**
 * save - save function
 * @args: command arguments
 * Return: status
 **/
int save(char **args)
{
	(void)args;
	printf("This is %s\n", args[0]);
	return (0);
}
/**
 * print_help - function
 **/
void print_help(void)
{
	printf("This is help\n");
}
/**
 * my_exit - my_exit function
 * @args: command arguments
 * Return: status
 **/
int my_exit(char **args)
{
	(void)args;
	printf("goodbye\n");
	running = 0;
	return (KEEP_PREVIOUS_STATUS);
}

/**
 * execute - execute function
 * @args: command arguments
 * Return: status
 **/
int execute(char **args)
{
	blockchain_cli_command_t commands[] = {
		{"wallet_load", wallet_load},
		{"wallet_save", wallet_save},
		{"send", send},
		{"mine", mine},
		{"info", info},
		{"load", load},
		{"save", save},
		{"exit", my_exit}
	};

	size_t i;

	if (!args || !(*args))
	{
		print_help();
		return (1);
	}

	for (i = 0; i < sizeof(commands) / sizeof(commands[0]); i++)
		if (!strcmp(args[0], commands[i].command))
			return (commands[i].func(args));

	fprintf(stderr, "%s: \"%s\": Unknown command\n", program_name, args[0]);
	print_help();
	return (1);
}

/**
 * realloc_array - reallocates an array
 * @arr: pointer to array
 * @arr_size: pointer to array size
 * Return: pointer to reallocated array
 **/
void *realloc_array(void *arr, size_t *arr_size)
{
	*arr_size += (3 * sizeof(char *));
	return (realloc(arr, *arr_size));
}

/**
 * getargs - function
 * @args: args
 * @args_size: size of args
 * @line: line where args are taken from
 * Return: status
 **/
char **getargs(char **args, size_t *args_size, char *line)
{
	size_t i;
	char *token;

	if (*args_size == 0)
		args = realloc_array(args, args_size);

	for (i = 0; (token = strtok(line, " \n")); i++)
	{
		line = NULL;
		if (i == *args_size)
			args = realloc_array(args, args_size);
		args[i] = token;
	}

	args[i] = NULL;
	return (args);
}
/**
 * main - entry point
 * @ac: arg count
 * @av: arg array
 * Return: status
 **/
int main(int ac, char **av)
{
	char **args = NULL;
	size_t args_size = 0;
	int prev_status, status = 0;

	program_name = av[0];

	if (ac > 1)
	{
		print_help();
		exit(1);
	}

	while (running)
	{
		printf("%s", default_prompt);

		memset(line, 0, line_size);
		if (getline(&line, &line_size, stdin) == -1)
		{
			status = error_out("getline");
		}
		else if (!(args = getargs(args, &args_size, line)))
		{
			status = error_out("getargs");
		}
		else if (args)
		{
			prev_status = status;
			status = execute(args);
			if (status == KEEP_PREVIOUS_STATUS)
				status = prev_status;
		}
	}
	free(line);
	free(args);
	return (status);
}
