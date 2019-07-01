CFLAGS=-std=c11 -g -static

9hcc: 9hcc.c

test: 9hcc
	./test.sh

clean:
	rm -f 9hcc *.o *~ tmp*

.PHONY: test clean
