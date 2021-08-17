#include "transaction.h"
#include <stdlib.h>

/**
 * transaction_destroy - deallocates a transaction structure
 * @transaction: pointer to transaction to destroy
 **/
void transaction_destroy(transaction_t *transaction)
{
	if (!transaction)
		return;

	if (transaction->inputs) llist_destroy(transaction->inputs, 1, free);
	if (transaction->outputs) llist_destroy(transaction->outputs, 1, free);
	free(transaction);
}
