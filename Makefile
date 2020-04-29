CFLAGS=-std=c11 -g -static
SRCS=$(wildcard src/*.c)
OBJS=$(SRCS:.c=.o)

9cc: $(OBJS)
	$(CC) -o bin/9hcc $(OBJS) $(LDFLAGS)

$(OBJS): src/9hcc.h

test: bin/9hcc
	./test.sh

clean:
	rm -f bin/9hcc src/*.o src/*~ src/tmp*

.PHONY: test clean