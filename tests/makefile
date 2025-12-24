CSTD:=99
CFLAGS:=
CC=cc
SRC = src
BLD = build
$(shell echo test)
TARGET=TestFullSuite
sources:=$(shell find src -name '*.c')
objs:=$(patsubst $(SRC)/%.c,$(OBJ)/%.o,$(sources))
$(shell $(CC) $(CFLAGS) -MMD -c src/main.c -o build/main.o)
deps:=$(OBJ)/main.d

UNITSRCS = $(wildcard $(SRC)/unit/*.c)

TESTS_BIN = $(patsubst $(SRC)/unit/%.c,$(BIN)/%.o,$(UNITSRCS))

ifeq ($(CC),pgcc)
        CFLAGS += -c$(CSTD)
else
        CFLAGS += -std=c$(CSTD)
endif

.PHONY: all
all: $(BLD)/fullsuite

$(BLD)/fullsuite : $(BIN)/munit.o $(BIN)/lb_fullsuite.o $(TESTS_BIN)
	$(CC) $(CFLAGS) -o $(BLD)/fullsuite $(BIN)/munit.o $(BIN)/lb_fullsuite.o
	
$(TESTS_BIN) : $(UNITSRCS)
	$(CC) $(CFLAGS) -c $< -o $@
	
$(BIN)/%.o : $(SRC)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

.PHONY: clean
clean :
	rm -rf $(BIN)/*.o
	rm -rf $(BLD)/fullsuite
