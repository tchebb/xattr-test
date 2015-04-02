# Makefile

CFLAGS := -Wall -Wextra

all: xattr-test

clean:
	-rm -f xattr-test *.o

.PHONY: clean
