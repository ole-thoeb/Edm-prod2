
SRC = graph.cpp main.cpp fordFulkerson.cpp
OBJECTS = graph.o main.o fordFulkerson.o
HEADERS = graph.h fordFulkerson.h
BIN = ford.out
PRODBIN = prodFord.out

CC = g++
CFLAGS = -g -Wall -std=c++17

$(BIN): $(SRC)
	$(CC) $(CFLAGS) -o $@ $(SRC)

.PHONY: clean
clean:
	rm -rf $(BIN) $(OBJECTS)

.PHONEY: production
production:
	$(CC) -std=c++17 -o $(PRODBIN) $(SRC)