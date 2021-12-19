CC=gcc
CCOPTS=-Wall -Wextra -Wno-unknown-pragmas -std=c2x -pedantic
LIBS=-lpthread

SRCS=$(wildcard *.c)
TARGETS=$(SRCS:.c=)

.PHONY: all clean pristine

all: $(TARGETS)

clean:
	rm -f $(TARGETS)

pristine: clean

%: %.c
	$(CC) $(CCOPTS) -o $@ $< $(LIBS)
