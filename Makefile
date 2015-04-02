# Makefile

CFLAGS := -Wall -Wextra

all: xattr-bug

clean:
	-rm -f xattr-bug *.o

.PHONY: clean
