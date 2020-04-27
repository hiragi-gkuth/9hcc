CFLAGS=-std=c11 -g -static
SRCS=$(wildcard *.c)
OBJS=$(SRCS:.c=.o)

9cc: $(OBJS)
	$(CC) -o ./.9hcc $(OBJS) $(LDFLAGS)

$(OBJS): 9hcc.h

test: 9hcc
	./test.sh

clean:
	rm -f ../9hcc ./*.o ./*~ ../tmp*

.PHONY: test clean