CSTD:=11
PDIR:=../../..
CC=cc
build=../../build
source=..
pbuild=$(PDIR)/build
psource=$(PDIR)/src
CFLAGS:=-I $(source) $(psource)

ifeq ($(CC),pgcc)
        CFLAGS += -c$(CSTD)
else
        CFLAGS += -std=c$(CSTD)
endif

.PHONY : all
all : string tty threads

.PHONY : reqs
reqs : $(build)/ $(build)/munit.o
	
$(build)/%.o: $(source)/%.c
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) -c $< -o $@

$(build)/ :
	mkdir -p $(build)

# .PHONY : wip
# %.o : $(build)/ wip $(build)/munit.o
# 	$(CC) $(CFLAGS) -c wip.c -o $(build)/lb_sysi/test_lb_string.o
# 	$(CC) $(CFLAGS) $(build)/path/wip.o $(build)/munit.o $(pbuild)/lb_sysi/lb_string.o -o test_lb_string

.PHONY : string
string : $(build)/ $(build)/lb_sysi/test_lb_string.o $(build)/munit.o
	$(CC) $(CFLAGS) -c test_lb_string.c -o $(build)/lb_sysi/test_lb_string.o
	$(CC) $(CFLAGS) $(build)/lb_sysi/test_lb_string.o $(build)/munit.o $(pbuild)/lb_sysi/lb_string.o -o test_lb_string

.PHONY : tty
tty : $(build)/ $(build)/lb_ui/test_lb_tty.o $(build)/munit.o
	$(CC) $(CFLAGS) -c test_lb_tty.c -o $(build)/lb_ui/test_lb_tty.o
	$(CC) $(CFLAGS) $(build)/lb_ui/test_lb_tty.o $(build)/munit.o $(pbuild)/lb_ui/lb_tty.o $(pbuild)/lb_ui/lb_string.o $(pbuild)/lb_ui/lb_formio.o -o test_lb_tty

.PHONY : threads
threads : $(build)/ $(build)/lb_sysi/test_lb_threads.o $(build)/munit.o
	$(CC) $(CFLAGS) -c test_lb_threads.c -o $(build)/lb_sysi/test_lb_threads.o
	$(CC) $(CFLAGS) $(build)/lb_sysi/test_lb_threads.o $(build)/munit.o $(pbuild)/lb_sysi/lb_threads.o -o test_lb_threads


.PHONY : clean
clean :
	rm -rf ./test_lb_string ./test_lb_tty
	rm -rf $(build)
