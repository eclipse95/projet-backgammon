CFLAGS=-g 
#-fdiagnostics-color=auto 
LDFLAGS=-lSDL2main -lSDL2 -lSDL2_ttf
CC=gcc
EXEC=test

all:$(EXEC)

test: interface.o
	$(CC) -o test interface.o $(CFLAGS) $(LDFLAGS)

interface.o: interface.c interface.h backgammon.h arbitre.h
	$(CC) -o interface.o -c interface.c -Wall $(CFLAGS) $(LDFLAGS)
	
clean:
	rm *.o
