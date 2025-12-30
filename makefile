sdir=src
bdir=build
CSTD=11
CFLAGS:=-I$(sdir)
CC=cc
TARGET=LifeBoat
lb_main=$(bdir)/lb_main.o

ssysi=$(sdir)/lb_sysi
bsysi=$(bdir)/lb_sysi
lb_formio=$(bsysi)/lb_formio.o
lb_string=$(bsysi)/lb_string.o
lb_threads=$(bsysi)/lb_threads.o

sui=$(sdir)/lb_ui
bui=$(bdir)/lb_ui
lb_view=$(bui)/lb_view.o
lb_tty=$(bui)/lb_tty.o

OBJ=$(lb_formio) $(lb_string) $(lb_threads)
OBJ+= $(lb_view) $(lb_tty)
OBJ+= $(lb_main)

TEST_TARGET=TestLifeBoat
tdir=tests
tsdir=$(tdir)/src
tudir=$(tsdir)/unit
tbdir=$(tdir)/build
tedir=$(tdir)/bin
TEST_OBJ=$(patsubst $(build)/%,$(tbdir)/test_%,$(OBJ))
TEST_OBJ+= $(tbdir)/munit.o
TEST_CFLAGS:=-I$(sdir) -I$(tsdir) -I$(tudir)
TEST_CSTD=17

ifeq ($(CC),pgcc)
        CFLAGS += -c$(CSTD)
        TEST_CFLAGS += -c$(TEST_CSTD)
else
        CFLAGS += -std=c$(CSTD)
        TEST_CFLAGS += -std=c$(TEST_CSTD)
endif

.PHONY : all
all : $(TARGET)

$(TARGET) : $(sdir)/main.c $(sdir)/main.h $(OBJ)
	@echo building $@ with $(CC)
	@$(CC) $(CFLAGS) $< $(OBJ) -o $(TARGET)

$(lb_main) : $(sdir)/lb_main.c $(sdir)/lb_main.h $(bdir)/
	@echo building $(*F) with $(CC)
	@$(CC) $(CFLAGS) -c $(sdir)/$(*F).c -o $@


$(lb_tty) : $(sui)/lb_tty.c $(sui)/lb_tty.h $(bui)/
	@echo building $(*F) with $(CC)
	@$(CC) $(CFLAGS) -c $(sui)/$(*F).c -o $@

$(lb_view) : $(sui)/lb_view.c $(sui)/lb_view.h $(bui)/
	@echo building $(*F) with $(CC)
	@$(CC) $(CFLAGS) -c $(sui)/$(*F).c -o $@


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

$(tedir)/ :
	@mkdir $@

.PHONY : tests
tests: $(TEST_TARGET) $(tedir)/test_lb_units $(tedir)/test_lb_full

$(TEST_TARGET) : $(tbdir)/test_lb_full.o $(tbdir)/ $(tedir)/ $(TEST_OBJ)
	@$(CC) $(TEST_CFLAGS) $< $(OBJ) -o $(TARGET)

$(tbdir)/%.o : $(tsdir)/%.c $(build)/%.o $(tbdir)/
	@echo building $(*F) with $(CC)
	@$(CC) $(TEST_CFLAGS) -c $< -o $@

$(tedir)/test_lb_full : $(tsdir)/test_lb_full.c $(TEST_OBJ) $(tbdir)/
	@echo building $(*F) with $(CC)
	@$(CC) $(TEST_CFLAGS) $< $(tsdir)/test_lb_units.o $(TEST_OBJ) -o $@

$(tbdir)/test_lb_full.o : $(tsdir)/test_lb_full.c $(TEST_OBJ) $(tbdir)/
	@echo building $(*F) with $(CC)
	@$(CC) $(TEST_CFLAGS) -c $< -o $@

$(tedir)/test_lb_units : $(tsdir)/test_lb_units.o $(TEST_OBJ) $(tbdir)/
	@echo building $(*F) with $(CC)
	@$(CC) $(TEST_CFLAGS) $< $(TEST_OBJ) -o $@

$(tbdir)/test_lb_units.o : $(tsdir)/test_lb_units.c $(TEST_OBJ) $(tbdir)/
	@echo building $(*F) with $(CC)
	@$(CC) $(TEST_CFLAGS) -c $< -o $@

$(tbdir)/munit.o : $(tsdir)/munit.c $(tbdir)/
	@echo building $(*F) with $(CC)
	@$(CC) $(TEST_CFLAGS) -c $< -o $@

$(tbdir)/test_lb_ui/%.o : $(tsdir)/test_lb_ui/%.c $(build)/lb_ui/%.o $(tbdir)/munit.o
	@echo building $(*F) with $(CC)
	@$(CC) $(TEST_CFLAGS) -c $< -o $@

$(tbdir)/test_lb_sysi/%.o : $(tudir)/test_lb_sysi/%.c $(build)/lb_sysi/%.o $(tbdir)/munit.o
	@echo building $(*F) with $(CC)
	@$(CC) $(TEST_CFLAGS) -c $< -o $@


.PHONY : cleanall
cleanall: clean cleantests

.PHONY : clean
clean :
	rm -rf build/
	rm -rf $(TARGET)

.PHONY : cleantests
cleantests:
	rm -rf $(tbdir)/ $(tedir)/
	rm -rf $(TEST_TARGET)
