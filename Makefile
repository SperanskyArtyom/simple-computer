all: make
	echo "mop"

.PHONY: clean run all
clean:
	rm -rf */*.o

run:
	# Commands