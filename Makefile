CC=gcc
CFLAGS=-std=c99 -pedantic -Wall -Wextra -g -lm

project: project.o ppmIO.o imageManip.o
	$(CC) project.o ppmIO.o imageManip.o -o project -lm

project.o: project.c ppmIO.h imageManip.h
	$(CC) $(CFLAGS) -c project.c

ppmIO.o: ppmIO.c ppmIO.h
	$(CC) $(CFLAGS) -c ppmIO.c

imageManip.o: imageManip.c imageManip.h ppmIO.h
	$(CC) $(CFLAGS) -c imageManip.c

clean:
	rm -f *.o project
