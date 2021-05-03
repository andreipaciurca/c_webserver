CC=gcc
CFLAGS=-g -Wall
HEADER=header
SRC=src
OBJ=obj
BIN=bin
LINK=-lssl -lcrypto

all: webserver

common.o: $(SRC)/common.c $(HEADER)/common.h
	$(CC) -c $(SRC)/common.c -o $(OBJ)/common.o

webserver: $(SRC)/webserver.c common.o
	$(CC) -o $(BIN)/webserver $(SRC)/webserver.c $(OBJ)/common.o $(LINK)

clean:
	rm -r $(OBJ)/* $(BIN)/*