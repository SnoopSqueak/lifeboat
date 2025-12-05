CC=cc
SRC = ./src
BLD = ./build

.PHONY: all
all: $(BLD)/main

$(BLD)/main : $(BLD)/main.o $(BLD)/conlist.o $(BLD)/logger.o
	$(CC) -o $(BLD)/LifeBoat $(BLD)/main.o $(BLD)/conlist.o $(BLD)/logger.o

$(BLD)/main.o : $(SRC)/main.c $(BLD)/logger.o $(BLD)/conlist.o
	$(CC) -c $(SRC)/main.c -o $(BLD)/main.o

$(BLD)/conlist.o : $(SRC)/conlist.c
	$(CC) -c $(SRC)/conlist.c -o $(BLD)/conlist.o
	
$(BLD)/logger.o : $(SRC)/logger.c
	$(CC) -c $(SRC)/logger.c -o $(BLD)/logger.o

.PHONY: clean
clean :
	rm -r $(BLD)/*.o
	rm -r $(BLD)/LifeBoat
