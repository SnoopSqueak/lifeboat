CSTD:=11
CFLAGS:=-I src
CC=cc
SRC=main.c lb_main.c lb_sysi/lb_formio.c lb_sysi/lb_threads.c lb_sysi/lb_string.c lb_ui/lb_tty.c lb_ui/lb_view.c
OBJ=$(patsubst %.c,build/%.o,$(SRC))
TARGET=LifeBoat

ifeq ($(CC),pgcc)
        CFLAGS += -c$(CSTD)
else
        CFLAGS += -std=c$(CSTD)
endif

.PHONY: all
all : $(TARGET)

$(TARGET) : build/ $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) -o $(TARGET)
	
build/%.o: src/%.c
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) -c $< -o $@
	
build/ :
	mkdir build
	
.PHONY : clean
clean :
	rm -rf build/
	rm -rf $(TARGET)
