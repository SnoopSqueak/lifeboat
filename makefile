CC=cc

main : main.o conlist.o logger.o
	$(CC) -o main main.o conlist.o logger.o

main.o : main.c
	$(CC) -c main.c conlist.c logger.c

conlist.o : conlist.c
	$(CC) -c conlist.c
	
logger.o : logger.c
	$(CC) -c logger.c

.PHONY: clean
clean :
	rm main main.o logger.o conlist.o
