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

$(TARGET) : build/ $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) -o $(TARGET)
	
build/%.o: src/%.c
	mkdir -p $(@D)
	$(CC) $(CFLAGS) -c $< -o $@
	
build/ :
	mkdir build
	
.PHONY : clean
clean :
	rm -rf build/
	rm -rf $(TARGET)
