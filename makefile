CSTD:=99
CFLAGS:=
CC=cc
SRC=$(shell find src -iname '*.c')
OBJ=$(patsubst src/%.c,build/%.o,$(SRC))
TARGET=LifeBoat

ifeq ($(CC),pgcc)
        CFLAGS += -c$(CSTD)
else
        CFLAGS += -std=c$(CSTD)
endif

.PHONY: all
all : $(TARGET)

.PHONY: info
info :
	echo "$(SRC)"
	echo "$(OBJ)"

$(TARGET) : $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) -o $(TARGET)
	
build/%.o: src/%.c
	$(CC) $(CFLAGS) -c $< -o $@
	
.PHONY : clean
clean :
	rm -rf $(OBJ)
	rm -rf $(TARGET)
