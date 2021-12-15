CC=gcc
CCOPTS=-Wall -Wextra -std=c2x -pedantic
LIBS=

SRCS=$(wildcard *.c)
TARGETS=$(SRCS:.c=)

.PHONY: all clean pristine

all: $(TARGETS)

clean:
	rm -f $(TARGETS)
	$(MAKE) -C atomics $@
	$(MAKE) -C threads $@

pristine: clean

%: %.c
	$(CC) $(CCOPTS) -o $@ $< $(LIBS)
