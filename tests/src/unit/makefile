CSTD:=11
PDIR:=../../..
CC=cc
build=../../build
source=..
pbuild=$(PDIR)/build
CFLAGS:=-I $(source)

ifeq ($(CC),pgcc)
        CFLAGS += -c$(CSTD)
else
        CFLAGS += -std=c$(CSTD)
endif

.PHONY : all
all : string tty
	
$(build)/lb_ui/%.o: $(source)/unit/%.c
	mkdir -p $(@D)
	$(CC) $(CFLAGS) -c $< -o $@

$(build)/munit.o : $(source)/munit.c
	$(CC) $(CFLAGS) -c $(source)/munit.c -o $(build)/munit.o
	
$(build)/ :
	mkdir -p $(build)
	
.PHONY : string
string : $(build)/ $(build)/lb_ui/test_lb_string.o $(build)/munit.o
	$(CC) $(CFLAGS) -c test_lb_string.c -o $(build)/lb_ui/test_lb_string.o
	$(CC) $(CFLAGS) $(build)/lb_ui/test_lb_string.o $(build)/munit.o $(pbuild)/lb_ui/lb_string.o -o test_lb_string

.PHONY : tty
tty : $(build)/ $(build)/lb_ui/test_lb_tty.o $(build)/munit.o
	$(CC) $(CFLAGS) -c test_lb_tty.c -o $(build)/lb_ui/test_lb_tty.o
	$(CC) $(CFLAGS) $(build)/lb_ui/test_lb_tty.o $(build)/munit.o $(pbuild)/lb_ui/lb_tty.o $(pbuild)/lb_ui/lb_string.o $(pbuild)/lb_ui/lb_formio.o -o test_lb_tty


.PHONY : clean
clean :
	rm -rf ./test_lb_string ./test_lb_tty
	rm -rf $(build)
