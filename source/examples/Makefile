CC=gcc
CCOPTS=-Wall -Wextra -std=c2x -pedantic
LIBS=-lpthread -latomic
#LIBS=-lpthread   # Use this if your platform doesn't support atomic

SRCS=$(wildcard *.c)
TARGETS=$(SRCS:.c=)

.PHONY: all clean pristine

all: $(TARGETS)

clean:
	rm -f $(TARGETS)

pristine: clean

%: %.c
	$(CC) $(CCOPTS) -o $@ $< $(LIBS)
