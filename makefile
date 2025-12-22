CSTD:=99
CFLAGS:=-MMD
CC=cc

SRC=src
OBJ=build
TARGET=LifeBoat
#~ modules:=lb_ui
sources:=$(shell find src -name '*.c')
objs:=$(patsubst $(SRC)/%.c,$(OBJ)/%.o,$(sources))

ifeq ($(CC),pgcc)
        CFLAGS += -c$(CSTD)
else
        CFLAGS += -std=c$(CSTD)
endif

.PHONY: all
all: $(TARGET)

$(TARGET) : $(objs)
	echo $(objs)
	$(CC) $(CFLAGS) -o $(TARGET) $(objs)

$(objs) : $(sources)
	$(CC) $(CFLAGS) -c $< -o $@
	
.PHONY: clean
clean :
	rm -rf $(objs)
	rm -rf $(TARGET)

#~ -include $(deps)
