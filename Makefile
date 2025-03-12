APP_NAME = pr02

APP_DIR = console
MYSC_DIR = mySimpleComputer
MYTERM_DIR = myTerm
INCLUDE_DIR = include

APP_PATH = $(APP_DIR)/$(APP_NAME)
MYSC_PATH = $(MYSC_DIR)/libmy_sc.a
MYTERM_PATH = $(MYTERM_DIR)/libmyTerm.a

APP_SOURCES = $(wildcard $(APP_DIR)/*.c)
APP_OBJECTS = $(APP_SOURCES:$(APP_DIR)/%.c=$(APP_DIR)/%.o)
MYSC_SOURCES = $(wildcard $(MYSC_DIR)/*.c)
MYTERM_SOURCES = $(wildcard $(MYTERM_DIR)/*.c)

CC = gcc
CFLAGS = -Wall -Wextra -I $(INCLUDE_DIR) -MMD
DEPS = $(APP_OBJECTS:.o=.d)
LIBS = $(MYSC_PATH) $(MYTERM_PATH)

all: $(APP_PATH)

$(APP_PATH): $(APP_OBJECTS) $(LIBS)
	$(CC) $(CFLAGS) $^ -o $@ 

$(APP_DIR)/%.o: $(APP_DIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

$(MYSC_PATH): $(MYSC_SOURCES)
	cd $(MYSC_DIR) && $(MAKE)

$(MYTERM_PATH): $(MYTERM_SOURCES)
	cd $(MYTERM_DIR) && $(MAKE)

-include $(DEPS)

.PHONY: clean run all
clean:
	find . -name "*.o" -type f -delete
	find . -name "*.a" -type f -delete
	find . -name "*.d" -type f -delete

run:
	./$(APP_PATH)