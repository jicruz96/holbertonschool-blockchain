CC               = gcc
LINK_DIRS        = ../crypto ../blockchain/v0.3
LINKS            = $(addprefix -l, ssl hblk_crypto hblk_blockchain crypto llist) -pthread
INCLUDE          = $(addprefix -I, . $(LINK_DIRS)/ provided/)
CFLAGS           = -g3 -Wall -Werror -Wextra -pedantic $(INCLUDE)

cli: main.o
	$(CC) $(CFLAGS) main.o -o cli $(addprefix -L, $(LINK_DIRS)) $(LINKS)
