CSTD:=99
CFLAGS:=-MMD
CC=cc

SRC=src
OBJ=build
TARGET=LifeBoat
sources:=$(shell find src -name '*.c')
objs:=$(patsubst $(SRC)/%.c,$(OBJ)/%.o,$(sources))
deps:=$(OBJ)/main.d

ifeq ($(CC),pgcc)
        CFLAGS += -c$(CSTD)
else
        CFLAGS += -std=c$(CSTD)
endif

.PHONY: all
all: $(TARGET)

info:
	echo $(sources)
	echo $(objs)

$(TARGET) : $(objs)
	$(CC) $(CFLAGS) -o $(TARGET) $(objs)
	
build/%.o : src/%.c
	$(CC) $(CFLAGS) -c $< -o $@ 
	
.PHONY: clean
clean :
	rm -rf $(objs)
	rm -rf $(TARGET)

-include $(deps)
