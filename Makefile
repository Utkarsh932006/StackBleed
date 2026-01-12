# Compiler settings
CC = gcc
# Flags:
# -g: Add debug symbols (useful for GDB/Valgrind)
# -Wall -Wextra: Enable standard warnings
# -fsanitize=address,undefined: The tools that SHOULD catch bugs (but miss this one)
CFLAGS = -Wall -Wextra -g -fsanitize=address,undefined

all: vulnerable secure

vulnerable: src/vulnerable.c
	$(CC) $(CFLAGS) -o vulnerable src/vulnerable.c

secure: src/secure.c
	$(CC) $(CFLAGS) -o secure src/secure.c

clean:
	rm -f vulnerable secure
