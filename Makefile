APP_NAME = pr02

APP_DIR = console
LIB_DIR = mySimpleComputer
INCLUDE_DIR = include

APP_PATH = $(APP_DIR)/$(APP_NAME)
LIB_PATH = $(LIB_DIR)/libmy_sc.a

APP_SOURCES = $(wildcard $(APP_DIR)/*.c)
APP_OBJECTS = $(APP_SOURCES:$(APP_DIR)/%.c=$(APP_DIR)/%.o)
LIB_SOURCES = $(wildcard $(LIB_DIR)/*.c)

CC = gcc
CFLAGS = -Wall -Wextra -I $(INCLUDE_DIR) -MMD
DEPS = $(APP_OBJECTS:.o=.d)

all: $(APP_PATH)

$(APP_PATH): $(APP_OBJECTS) $(LIB_PATH)
	$(CC) $(CFLAGS) $^ -o $@ 

$(APP_DIR)/%.o: $(APP_DIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

$(LIB_PATH): $(LIB_SOURCES)
	cd mySimpleComputer && $(MAKE)

-include $(DEPS)

.PHONY: clean run all
clean:
	find . -name "*.o" -type f -delete
	find . -name "*.a" -type f -delete
	find . -name "*.d" -type f -delete

run:
	./$(APP_PATH)