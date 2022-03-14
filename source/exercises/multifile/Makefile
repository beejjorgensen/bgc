.PHONY: clean

runner: runner.o total.o
	gcc -o $@ $^

runner.o: runner.c total.h
	gcc -Wall -Wextra -std=c2x -pedantic -c $<

total.o: total.c
	gcc -Wall -Wextra -std=c2x -pedantic -c $<

clean:
	rm -f runner *.o
