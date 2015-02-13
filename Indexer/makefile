COMPILER = gcc
all: indexer

debug:
	make DEBUG=TRUE

indexer: main.o
	$(COMPILER) -o indexer main.o 
main.o: main.c main.h 
	$(COMPILER) -c main.c
clean:
	rm -rf indexer
	rm -rf *.o

rebuild: clean all
