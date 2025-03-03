LIBRARY = libmy_sc.a

SOURCES = $(wildcard *.c)
OBJECTS = $(SOURCES:.c=.o)

INCLUDE_PATH = ../include

CC = gcc
CFLAGS = -Wall -g -I $(INCLUDE_PATH)

all: $(LIBRARY)

$(LIBRARY): $(OBJECTS)
	ar rcs $@ $^

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

.PHONY: all