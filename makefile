sdir=src
bdir=build
CSTD=11
CFLAGS:=-I$(sdir)
CC=cc
TARGET=LifeBoat
lb_main=$(bdir)/lb_main.o

ssysi=$(sdir)/lb_sysi
bsysi=$(bdir)/lb_sysi
lb_string=$(bsysi)/lb_string.o
lb_threads=$(bsysi)/lb_threads.o

sui=$(sdir)/lb_ui
bui=$(bdir)/lb_ui
lb_view=$(bui)/lb_view.o
lb_tty=$(bui)/lb_tty.o

OBJ=$(lb_string) $(lb_threads)
OBJ+= $(lb_view) $(lb_tty)

TEST_TARGET=TestLifeBoat
tdir=tests
tsdir=$(tdir)/src
tudir=$(tsdir)/unit
tbdir=$(tdir)/build
tedir=$(tdir)/bin
testlib=$(tbdir)/munit.o
TEST_OBJ=$(patsubst $(bdir)/%.o,$(tbdir)/%.o,$(OBJ))
tmp:=$(subst lb_,test_lb_,$(TEST_OBJ))
TEST_OBJ=$(tmp)
TEST_OBJ+= $(testlib)
TEST_CFLAGS:=-I$(sdir) -I$(tsdir) -I$(tudir)
TEST_CSTD=17

OBJ+= $(lb_main)

ifeq ($(CC),pgcc)
        CFLAGS += -c$(CSTD)
        TEST_CFLAGS += -c$(TEST_CSTD)
else
        CFLAGS += -std=c$(CSTD)
        TEST_CFLAGS += -std=c$(TEST_CSTD)
endif

.PHONY : all
all : $(TARGET) $(TEST_TARGET)

$(TARGET) : $(sdir)/main.c $(sdir)/main.h $(OBJ)
	@echo building $@ with $(CC)!
	@$(CC) $(CFLAGS) $< $(OBJ) -o $@

$(lb_main) : $(sdir)/lb_main.c $(sdir)/lb_main.h $(bdir)/
	@echo building $(*F) with $(CC)
	@$(CC) $(CFLAGS) -c $< -o $@


$(lb_tty) : $(sui)/lb_tty.c $(sui)/lb_tty.h $(bui)/
	@echo building $(*F) with $(CC)
	@$(CC) $(CFLAGS) -c $< -o $@

$(lb_view) : $(sui)/lb_view.c $(sui)/lb_view.h $(bui)/
	@echo building $(*F) with $(CC)
	@$(CC) $(CFLAGS) -c $< -o $@


$(lb_threads) : $(ssysi)/lb_threads.c $(ssysi)/lb_threads.h $(bsysi)/
	@echo building $(*F) with $(CC)
	@$(CC) $(CFLAGS) -c $(ssysi)/$(*F).c -o $@

$(lb_string) : $(ssysi)/lb_string.c $(ssysi)/lb_string.h $(bsysi)/
	@echo building $(*F) with $(CC)
	@$(CC) $(CFLAGS) -c $(ssysi)/$(*F).c -o $@

$(lb_formio) : $(ssysi)/lb_formio.c $(ssysi)/lb_formio.h $(bsysi)/
	@echo building $(*F) with $(CC)
	@$(CC) $(CFLAGS) -c $(ssysi)/$(*F).c -o $@


$(bdir)/ :
	@mkdir $@

$(bdir)/%/ : $(bdir)/
	@mkdir -p $@

$(tbdir)/ :
	@mkdir $@

$(tbdir)/%/ : $(tbdir)/
	@mkdir -p $@

$(tedir)/ :
	@mkdir $@

$(tedir)/%/ : $(tedir)/
	@mkdir -p $@

.PHONY : tests
tests: $(tedir)/ $(tedir)/test_lb_units $(tedir)/test_lb_full $(TEST_TARGET)

$(TEST_TARGET) : $(tsdir)/test_lb_full.c $(tbdir)/ $(TEST_OBJ) $(bdir)/lb_main.o
	@echo building $@ with $(CC)!
	@$(CC) $(TEST_CFLAGS) $< $(OBJ) $(testlib) -o $(TEST_TARGET)

$(tedir)/test_lb_full : $(tsdir)/test_lb_full.c $(tbdir)/test_lb_units.o $(TEST_OBJ) $(tedir)/
	@echo building bin test_lb_full with $(CC)
	@$(CC) $(TEST_CFLAGS) $< $(testlib) $(OBJ) -o $@

$(tbdir)/test_lb_full.o : $(tsdir)/test_lb_full.c $(TEST_OBJ) $(tbdir)/
	@echo building $(*F) with $(CC)
	@$(CC) $(TEST_CFLAGS) -c $< -o $@

$(tedir)/test_lb_units : $(tbdir)/test_lb_units.o $(TEST_OBJ) $(OBJ) $(tedir)/
	@echo building bin test_lb_units with $(CC)
	@$(CC) $(TEST_CFLAGS) $< $(OBJ) $(testlib) -o $@

$(tbdir)/test_lb_units.o : $(tsdir)/test_lb_units.c $(TEST_OBJ) $(tbdir)/
	@echo building $(*F) with $(CC)
	@$(CC) $(TEST_CFLAGS) -c $< -o $@

$(tbdir)/munit.o : $(tsdir)/munit.c $(tbdir)/
	@echo building $(*F) with $(CC)
	@$(CC) $(TEST_CFLAGS) -c $< -o $@

$(tbdir)/test_lb_ui/test_%.o : $(tudir)/test_lb_ui/test_%.c $(tbdir)/test_lb_ui/ $(bdir)/lb_ui/%.o $(testlib)
	@echo building test_$(*F) with $(CC)
	@$(CC) $(TEST_CFLAGS) -c $< -o $@

$(tbdir)/test_lb_sysi/test_%.o : $(tudir)/test_lb_sysi/test_%.c $(tbdir)/test_lb_sysi/ $(bdir)/lb_sysi/%.o $(testlib)
	@echo building test_$(*F) with $(CC)
	@$(CC) $(TEST_CFLAGS) -c $< -o $@

$(tedir)/test_lb_sysi/test_lb_string : $(tbdir)/test_lb_sysi/test_lb_string.o $(bdir)/lb_sysi/lb_string.o $(tbdir)/munit.o $(tedir)/test_lb_sysi/
	@echo building bin test_lb_string with $(CC)
	@$(CC) $(TEST_CFLAGS) $< $(bdir)/lb_sysi/lb_string.o $(testlib) -o $@


.PHONY : cleanall
cleanall: clean cleantests

.PHONY : clean
clean :
	rm -rf build/
	rm -rf $(TARGET)

.PHONY : cleantests
cleantests:
	rm -rf $(tbdir)/ $(tedir)/
	rm -rf lb_testfile.txt
	rm -rf $(TEST_TARGET)
