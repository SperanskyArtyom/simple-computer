all: console/test

console/test: console/test.o

console/test.o: console/test.c

.PHONY: clean run
clean:
	rm -rf */*.o */test

run:
	./console/test