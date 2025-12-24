CSTD:=99
PDIR:=..
CC=cc
build=build
source=src
CFLAGS:=-I $(source)
TARGET=TestFullSuite
psource=../src
PSRC=$(psource)/lb_ui/lb_string.c
pbuild=../build
POBJ=$(patsubst $(psource)/%.c,$(pbuild)/%.o,$(PSRC))
SRC=$(source)/munit.c $(source)/lb_fullsuite.c $(source)/lb_units.h $(source)/unit/test_lb_string.h
OBJ=$(patsubst $(source)/%.c,$(build)/%.o,$(SRC))

ifeq ($(CC),pgcc)
        CFLAGS += -c$(CSTD)
else
        CFLAGS += -std=c$(CSTD)
endif

.PHONY : all
all : $(TARGET)

$(TARGET) : $(build)/ $(OBJ) $(POBJ)
	$(CC) $(CFLAGS) $(OBJ) $(POBJ) -o $@
	
$(build)/munit.o : $(source)/munit.c
	$(CC) $(CFLAGS) -c $(source)/munit.c -o $(build)/munit.o
	
$(build)/%.o: $(source)/%.c
	mkdir -p $(@D)
	$(CC) $(CFLAGS) -c $< -o $@
	
$(build)/lb_ui/%.o: $(source)/unit/%.c
	mkdir -p $(@D)
	$(CC) $(CFLAGS) -c $< -o $@
	
$(build)/lb_ui/ :
	mkdir -p $(build)/lb_ui
	
$(build)/ :
	mkdir -p $(build)
	
.PHONY : clean
clean :
	rm -rf $(TARGET)
	rm -rf $(build)
