CC               = gcc
LINK_DIRS        = ../crypto ../blockchain/v0.3
LINKS            = $(addprefix -l, ssl hblk_crypto crypto llist hblk_blockchain) -pthread
INCLUDE          = $(addprefix -I, . $(LINK_DIRS)/)
CFLAGS           = -g3 -Wall -Werror -Wextra -pedantic $(INCLUDE)

cli:
	$(CC) $(CFLAGS) main.c -o cli $(addprefix -L, $(LINK_DIRS)) $(LINKS)
