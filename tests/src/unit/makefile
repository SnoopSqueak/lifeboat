CSTD:=99
PDIR=../../..
CFLAGS:=-I $(PDIR)/src
CC=cc
srcs:=$(wildcard *.c)
objs:=$(patsubst %.c,%.o,$(srcs))
bins:=$(patsubst %.c,%,$(srcs))
OBJ=../../build

ifeq ($(CC),pgcc)
        CFLAGS += -c$(CSTD)
else
        CFLAGS += -std=c$(CSTD)
endif

.PHONY: all
all: $(bins)

$(bins) : $(OBJ)/$(objs) $(OBJ)/munit.o
	$(CC) $(CFLAGS) -o $@ $< $(OBJ)/munit.o

$(OBJ)/munit.o : ../munit.c
	$(CC) $(CFLAGS) -c ../munit.c -o $(OBJ)/munit.o

$(OBJ)/%.o : %.c
	$(CC) $(CFLAGS) -c $< -o $(OBJ)/$@
	
.PHONY: string
string : $(OBJ)/test_lb_string.o $(OBJ)/munit.o
	$(CC) $(CFLAGS) -c test_lb_string.c -o $(OBJ)/test_lb_string.o
	$(CC) $(CFLAGS) -o tl_string $(OBJ)/test_lb_string.o $(OBJ)/munit.o $(PDIR)/build/lb_string.o

.PHONY: clean
clean :
	rm -rf $(OBJ)/*.o
	rm -rf ./tl_string ./test_lb_string
