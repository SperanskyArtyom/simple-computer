all:
	cd mySimpleComputer && $(MAKE)

.PHONY: clean run all
clean:
	find . -name "*.o" -type f -delete
	find . -name "*.a" -type f -delete

run:
	# Commands