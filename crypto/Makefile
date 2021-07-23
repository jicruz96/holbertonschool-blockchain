# ~*~ Makefile ~*~
CC       = gcc
CFLAGS   = -g3 -Wall -Werror -Wextra -pedantic -I.
LINKS    = -lssl -lcrypto
PRINTER  = provided/_print_hex_buffer.c

T1 = sha256
$(T1)_files       = $(T1).c test/$(T1)-main.c $(PRINTER)
$(T1)_output_file = $(T1)-test
$(T1): $($(T1)_files:.c=.o)
	$(CC) $(CFLAGS) $($(T1)_files) $(LINKS) -o $($(T1)_output_file)

T2 = ec_create
$(T2)_files       = $(T2).c test/$(T2)-main.c $(PRINTER)
$(T2)_output_file = $(T2)-test
$(T2): $($(T2)_files:.c=.o)
	$(CC) $(CFLAGS) $($(T2)_files) $(LINKS) -o $($(T2)_output_file)

T3 = ec_to_pub
$(T3)_files       = $(T3).c ec_create.c test/$(T3)-main.c $(PRINTER)
$(T3)_output_file = $(T3)-test
$(T3): $($(T3)_files:.c=.o)
	$(CC) $(CFLAGS) $($(T3)_files) $(LINKS) -o $($(T3)_output_file)

T3 = ec_from_pub
$(T3)_files       = $(T3).c ec_to_pub.c ec_create.c test/$(T3)-main.c $(PRINTER)
$(T3)_output_file = $(T3)-test
$(T3): $($(T3)_files:.c=.o)
	$(CC) $(CFLAGS) $($(T3)_files) $(LINKS) -o $($(T3)_output_file)
