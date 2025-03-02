LIBRARY = libmy_sc.a

SOURCES = $(wildcard *.c)
OBJECTS = $(SOURCES:.c=.o)

CC = gcc
CFLAGS = -Wall -g

all: $(LIBRARY)

$(LIBRARY): $(OBJECTS)
	ar rcs $@ $^

$(OBJECTS): $(SOURCES)
	$(CC) $(CFLAGS) -c $< -o $@

.PHONY: all