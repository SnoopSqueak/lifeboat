CSTD:=99
CFLAGS:=-I "../../../src"
CC=cc
srcs:=$(wildcard *.c)
objs:=$(patsubst %.c,%.o,$(srcs))
bins:=$(patsubst %.c,%,$(srcs))

ifeq ($(CC),pgcc)
        CFLAGS += -c$(CSTD)
else
        CFLAGS += -std=c$(CSTD)
endif

.PHONY: all
all: $(bins)

$(bins) : $(objs) ../munit.o
	$(CC) $(CFLAGS) -o $@ $< ../munit.o

../munit.o : ../munit.c
	$(CC) $(CFLAGS) -c ../munit.c -o ../munit.o

%.o : %.c
	$(CC) $(CFLAGS) -c $< -o $@
	
string : test_lb_string.o ../munit.o
	$(CC) $(CFLAGS) -c test_lb_string.c -o test_lb_string.o
	$(CC) $(CFLAGS) -o tl_string test_lb_string.o ../munit.o ../../../build/lb_string.o

.PHONY: clean
clean :
	rm -rf ./*.o
	rm -rf ../munit.o
	rm -rf ./tl_string ./test_lb_string
