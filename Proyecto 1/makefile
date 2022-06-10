TARGET = tpe
OBJS = dominoFront.o dominoBack.o pool.o list.o getnum.o
CC = gcc
CFLAGS = -Wall -pedantic -g 

$(TARGET):	$(OBJS) 
	$(CC) -o$(TARGET) $(OBJS) $(CFLAGS) 

dominoBack.o: dominoBack.h pool.h list.h

dominoFront.o: dominoBack.h getnum.h

pool.c: pool.h

list.o: list.h

getnum.o: getnum.h
