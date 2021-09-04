CC               = gcc
LINK_DIRS        = ../crypto ../blockchain/v0.3 ../blockchain/v0.3/transaction
LINKS            = $(addprefix -l, hblk_blockchain llist ssl crypto hblk_crypto) -pthread
INCLUDE          = $(addprefix -I, . $(LINK_DIRS)/)
CFLAGS           = -g3 -Wall -Werror -Wextra -pedantic $(INCLUDE)

cli:
	$(CC) $(CFLAGS) main.c -o cli $(addprefix -L, $(LINK_DIRS)) -Wl,--start-group $(LINKS) -Wl,--end-group
