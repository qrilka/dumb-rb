
all:
	gcc -g ./tests.c ./ringbuf.c -o ./tests

filter: filter.c ringbuf.c
	gcc -g -lpthread ./filter.c ./ringbuf.c -o ./filter

test-filter: filter
	./test-filter.sh

clean:
	git clean -f -d

