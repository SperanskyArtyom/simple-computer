all: test

test: console/test.o
	gcc -o test console/test.c

.PHONY: clean
clean:
	rm -rf console/*.o
