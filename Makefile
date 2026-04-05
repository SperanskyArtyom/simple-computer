APP_NAME = console
FONT_NAME = font
SAT_NAME = sat

APP_DIR = console
MYSC_DIR = mySimpleComputer
MYTERM_DIR = myTerm
MYBC_DIR = myBigChars
MYRK_DIR = myReadKey
SIMPLE_ASSEMBLER_DIR = simpleassembler
INCLUDE_DIR = include

APP_PATH = $(APP_DIR)/$(APP_NAME)
FONT_PATH = $(APP_DIR)/$(FONT_NAME)
SAT_PATH = $(SIMPLE_ASSEMBLER_DIR)/$(SAT_NAME)
MYSC_PATH = $(MYSC_DIR)/libmy_sc.a
MYTERM_PATH = $(MYTERM_DIR)/libmyTerm.a
MYBC_PATH = $(MYBC_DIR)/libmy_bc.a
MYRK_PATH = $(MYRK_DIR)/libmy_rk.a

# Исключаем font.c из списка исходников для console
APP_SOURCES = $(filter-out $(APP_DIR)/font.c, $(wildcard $(APP_DIR)/*.c))
APP_OBJECTS = $(APP_SOURCES:$(APP_DIR)/%.c=$(APP_DIR)/%.o)

FONT_SOURCES = $(APP_DIR)/font.c
FONT_OBJECTS = $(FONT_SOURCES:$(APP_DIR)/%.c=$(APP_DIR)/%.o)

SAT_SOURCES = $(SIMPLE_ASSEMBLER_DIR)/translator.c
SAT_OBJECTS = $(SAT_SOURCES:$(SIMPLE_ASSEMBLER_DIR)/%.c=$(SIMPLE_ASSEMBLER_DIR)/%.o)

MYSC_SOURCES = $(wildcard $(MYSC_DIR)/*.c)
MYTERM_SOURCES = $(wildcard $(MYTERM_DIR)/*.c)
MYBC_SOURCES = $(wildcard $(MYBC_DIR)/*.c)
MYRK_SOURCES = $(wildcard $(MYRK_DIR)/*.c)

CC = gcc
CFLAGS = -Wall -Wextra -I $(INCLUDE_DIR) -MMD
DEPS = $(APP_OBJECTS:.o=.d) $(FONT_OBJECTS:.o=.d) $(SAT_OBJECTS:.o=.d)
LIBS = $(MYSC_PATH) $(MYTERM_PATH) $(MYBC_PATH) $(MYRK_PATH)

# Порядок библиотек важен! myBigChars зависит от myTerm, поэтому myTerm должен быть после
LIB_ORDER = -L$(MYSC_DIR) -lmy_sc -L$(MYTERM_DIR) -lmyTerm -L$(MYBC_DIR) -lmy_bc -L$(MYRK_DIR) -lmy_rk

all: $(APP_PATH) $(FONT_PATH) $(SAT_PATH)

$(APP_PATH): $(APP_OBJECTS) $(LIBS)
	$(CC) $(CFLAGS) $^ $(LIB_ORDER) -o $@ 

$(FONT_PATH): $(FONT_OBJECTS) $(LIBS)
	$(CC) $(CFLAGS) $^ $(LIB_ORDER) -o $@

$(SAT_PATH): $(SAT_OBJECTS) $(MYSC_PATH)
	$(CC) $(CFLAGS) $^ -L$(MYSC_DIR) -lmy_sc -o $@

$(APP_DIR)/%.o: $(APP_DIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

$(SIMPLE_ASSEMBLER_DIR)/%.o: $(SIMPLE_ASSEMBLER_DIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

$(MYSC_PATH): $(MYSC_SOURCES)
	cd $(MYSC_DIR) && $(MAKE)

$(MYTERM_PATH): $(MYTERM_SOURCES)
	cd $(MYTERM_DIR) && $(MAKE)

$(MYBC_PATH): $(MYBC_SOURCES)
	cd $(MYBC_DIR) && $(MAKE)

$(MYRK_PATH): $(MYRK_SOURCES)
	cd $(MYRK_DIR) && $(MAKE)

-include $(DEPS)

.PHONY: clean run all font sat
clean:
	find . -name "*.o" -type f -delete
	find . -name "*.a" -type f -delete
	find . -name "*.d" -type f -delete
	rm -f $(FONT_PATH) $(APP_PATH) $(SAT_PATH)

run:
	./$(APP_PATH)

font: 
	./$(FONT_PATH)