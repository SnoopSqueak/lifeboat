CC=cc
SRC = ./src
BLD = ./build

.PHONY: all
all: $(BLD)/main

$(BLD)/main : $(BLD)/main.o $(BLD)/conlist.o $(BLD)/lb_term.o
	$(CC) -o $(BLD)/LifeBoat $(BLD)/main.o $(BLD)/conlist.o $(BLD)/lb_term.o

$(BLD)/main.o : $(SRC)/main.c $(BLD)/lb_term.o $(BLD)/conlist.o
	$(CC) -c $(SRC)/main.c -o $(BLD)/main.o

$(BLD)/conlist.o : $(SRC)/conlist.c
	$(CC) -c $(SRC)/conlist.c -o $(BLD)/conlist.o
	
$(BLD)/lb_term.o : $(SRC)/lb_term.c
	$(CC) -c $(SRC)/lb_term.c -o $(BLD)/lb_term.o
	
reset: $(BLD)/reset_color

$(BLD)/reset_color : $(BLD)/reset_color.o
	$(CC) -o $(BLD)/ResetColor $(BLD)/reset_color.o

$(BLD)/reset_color.o : $(SRC)/reset_color.c
	$(CC) -c $(SRC)/reset_color.c -o $(BLD)/reset_color.o

.PHONY: clean
clean :
	rm -rf $(BLD)/*.o
	rm -rf $(BLD)/LifeBoat
