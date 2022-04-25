

MAINS= stree
CC=gcc


all: $(MAINS)

#c files are compiled to o files authomaticlly.

stree: stree.o 
	$(CC) -o stree stree.o


.PHONY: clean all
clean:
	rm *.o $(MAINS) 

