

MAINS= slast
CC=gcc


all: $(MAINS)

#c files are compiled to o files authomaticlly.

slast: slast.o 
	$(CC) -o slast slast.o


.PHONY: clean all
clean:
	rm *.o $(MAINS) 

