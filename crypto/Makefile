# ~*~ Makefile ~*~
CC       = gcc
CFLAGS   = -g3 -Wall -Werror -Wextra -pedantic -I.
LINKS    = -lssl -lcrypto
PRINTER  = provided/_print_hex_buffer.c

T0 = sha256
$(T0)_files       = $(T0).c test/$(T0)-main.c $(PRINTER)
$(T0): $($(T0)_files:.c=.o)
	$(CC) $(CFLAGS) $($(T0)_files) $(LINKS) -o $(T0)-test

T1 = ec_create
$(T1)_files       = $(T1).c test/$(T1)-main.c $(PRINTER)
$(T1): $($(T1)_files:.c=.o)
	$(CC) $(CFLAGS) $($(T1)_files) $(LINKS) -o $(T1)-test

T2 = ec_to_pub
$(T2)_files       = $(T2).c ec_create.c test/$(T2)-main.c $(PRINTER)
$(T2): $($(T2)_files:.c=.o)
	$(CC) $(CFLAGS) $($(T2)_files) $(LINKS) -o $(T2)-test

T3 = ec_from_pub
$(T3)_files       = $(T3).c ec_to_pub.c ec_create.c test/$(T3)-main.c $(PRINTER)
$(T3): $($(T3)_files:.c=.o)
	$(CC) $(CFLAGS) $($(T3)_files) $(LINKS) -o $(T3)-test

T4 = ec_save
$(T4)_files       = $(T4).c ec_to_pub.c ec_create.c test/$(T4)-main.c $(PRINTER)
$(T4): $($(T4)_files:.c=.o)
	$(CC) $(CFLAGS) $($(T4)_files) $(LINKS) -o $(T4)-test

T5 = ec_load
$(T5)_files       = $(T5).c ec_save.c ec_to_pub.c ec_create.c test/$(T5)-main.c $(PRINTER)
$(T5): $($(T5)_files:.c=.o)
	$(CC) $(CFLAGS) $($(T5)_files) $(LINKS) -o $(T5)-test

T6 = ec_sign
$(T6)_files       = $(T6).c ec_create.c test/$(T6)-main.c $(PRINTER)
$(T6): $($(T6)_files:.c=.o)
	$(CC) $(CFLAGS) $($(T6)_files) $(LINKS) -o $(T6)-test

T7 = ec_verify
$(T7)_files       = $(T7).c ec_sign.c ec_create.c test/$(T7)-main.c $(PRINTER)
$(T7): $($(T7)_files:.c=.o)
	$(CC) $(CFLAGS) $($(T7)_files) $(LINKS) -o $(T7)-test


all_files = $(addsuffix .o,$(T0) $(T1) $(T2) $(T3) $(T4) $(T5) $(T6) $(T7))
libhblk_crypto.a: $(all_files)
	ar rcs libhblk_crypto.a $(all_files)

other_files = $(addsuffix .o, $(addprefix ../blockchain/v0.3/transaction/, tx_out_create unspent_tx_out_create tx_in_create transaction_hash tx_in_sign transaction_create transaction_is_valid coinbase_create coinbase_is_valid transaction_destroy update_unspent)) $(all_files)
libhblk_blockchain.a: $(other_files)
	ar rcs libhblk_blockchain.a $(other_files)
