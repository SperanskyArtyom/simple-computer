APP_NAME = console
FONT_NAME = font

APP_DIR = console
MYSC_DIR = mySimpleComputer
MYTERM_DIR = myTerm
MYBC_DIR = myBigChars
INCLUDE_DIR = include

APP_PATH = $(APP_DIR)/$(APP_NAME)
FONT_PATH = $(APP_DIR)/$(FONT_NAME)
MYSC_PATH = $(MYSC_DIR)/libmy_sc.a
MYTERM_PATH = $(MYTERM_DIR)/libmyTerm.a
MYBC_PATH = $(MYBC_DIR)/libmy_bc.a

# Исключаем font.c из списка исходников для console
APP_SOURCES = $(filter-out $(APP_DIR)/font.c, $(wildcard $(APP_DIR)/*.c))
APP_OBJECTS = $(APP_SOURCES:$(APP_DIR)/%.c=$(APP_DIR)/%.o)

FONT_SOURCES = $(APP_DIR)/font.c
FONT_OBJECTS = $(FONT_SOURCES:$(APP_DIR)/%.c=$(APP_DIR)/%.o)

MYSC_SOURCES = $(wildcard $(MYSC_DIR)/*.c)
MYTERM_SOURCES = $(wildcard $(MYTERM_DIR)/*.c)
MYBC_SOURCES = $(wildcard $(MYBC_DIR)/*.c)

CC = gcc
CFLAGS = -Wall -Wextra -I $(INCLUDE_DIR) -MMD
DEPS = $(APP_OBJECTS:.o=.d) $(FONT_OBJECTS:.o=.d)
LIBS = $(MYSC_PATH) $(MYTERM_PATH) $(MYBC_PATH)

# Порядок библиотек важен! myBigChars зависит от myTerm, поэтому myTerm должен быть после
LIB_ORDER = -L$(MYSC_DIR) -lmy_sc -L$(MYTERM_DIR) -lmyTerm -L$(MYBC_DIR) -lmy_bc

all: $(APP_PATH) $(FONT_PATH)

$(APP_PATH): $(APP_OBJECTS) $(LIBS)
	$(CC) $(CFLAGS) $^ $(LIB_ORDER) -o $@ 

$(FONT_PATH): $(FONT_OBJECTS) $(LIBS)
	$(CC) $(CFLAGS) $^ $(LIB_ORDER) -o $@

$(APP_DIR)/%.o: $(APP_DIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

$(MYSC_PATH): $(MYSC_SOURCES)
	cd $(MYSC_DIR) && $(MAKE)

$(MYTERM_PATH): $(MYTERM_SOURCES)
	cd $(MYTERM_DIR) && $(MAKE)

$(MYBC_PATH): $(MYBC_SOURCES)
	cd $(MYBC_DIR) && $(MAKE)

-include $(DEPS)

.PHONY: clean run all font
clean:
	find . -name "*.o" -type f -delete
	find . -name "*.a" -type f -delete
	find . -name "*.d" -type f -delete
	rm -f $(FONT_PATH) $(APP_PATH)

run:
	./$(APP_PATH)

font: $(FONT_PATH)