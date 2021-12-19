CC=gcc
CCOPTS=-Wall -Wextra -Wno-unknown-pragmas -std=c2x -pedantic
LIBS=-lm

SRCS=$(wildcard *.c)
TARGETS=$(SRCS:.c=)

.PHONY: all clean pristine

all: $(TARGETS)

tmpnam: tmpnam.c
	$(CC) $(CCOPTS) -Wno-deprecated -o $@ $< $(LIBS)

fflush: fflush.c
	$(CC) $(CCOPTS) -Wno-deprecated -o $@ $< $(LIBS) -lpthread

clean:
	rm -f $(TARGETS)
	$(MAKE) -C atomics $@
	$(MAKE) -C threads $@

pristine: clean

%: %.c
	$(CC) $(CCOPTS) -o $@ $< $(LIBS)
