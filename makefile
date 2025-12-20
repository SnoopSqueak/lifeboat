CC=cc
SRC = ./src
BLD = ./build

.PHONY: all
all: $(BLD)/main

$(BLD)/main : $(BLD)/main.o $(BLD)/lb_main.o $(BLD)/conlist.o $(BLD)/lb_tty.o $(BLD)/lb_state.o $(BLD)/lb_ui.o $(BLD)/lb_string.o
	$(CC) -o $(BLD)/LifeBoat $(BLD)/main.o $(BLD)/lb_main.o $(BLD)/conlist.o $(BLD)/lb_tty.o $(BLD)/lb_state.o $(BLD)/lb_ui.o $(BLD)/lb_string.o

$(BLD)/main.o : $(SRC)/main.c $(BLD)/lb_tty.o $(BLD)/conlist.o
	$(CC) -c $(SRC)/main.c -o $(BLD)/main.o

$(BLD)/conlist.o : $(SRC)/conlist.c
	$(CC) -c $(SRC)/conlist.c -o $(BLD)/conlist.o
	
$(BLD)/lb_main.o : $(SRC)/lb_main.c
	$(CC) -c $(SRC)/lb_main.c -o $(BLD)/lb_main.o
	
$(BLD)/lb_tty.o : $(SRC)/lb_tty.c
	$(CC) -c $(SRC)/lb_tty.c -o $(BLD)/lb_tty.o
	
$(BLD)/lb_ui.o : $(SRC)/lb_ui.c
	$(CC) -c $(SRC)/lb_ui.c -o $(BLD)/lb_ui.o

$(BLD)/lb_state.o : $(SRC)/lb_state.c
	$(CC) -c $(SRC)/lb_state.c -o $(BLD)/lb_state.o

$(BLD)/lb_string.o : $(SRC)/lb_string.c
	$(CC) -c $(SRC)/lb_string.c -o $(BLD)/lb_string.o
	
reset: $(BLD)/reset_color

$(BLD)/reset_color : $(BLD)/reset_color.o
	$(CC) -o $(BLD)/ResetColor $(BLD)/reset_color.o

$(BLD)/reset_color.o : $(SRC)/reset_color.c
	$(CC) -c $(SRC)/reset_color.c -o $(BLD)/reset_color.o

.PHONY: clean
clean :
	rm -rf $(BLD)/*.o
	rm -rf $(BLD)/LifeBoat
