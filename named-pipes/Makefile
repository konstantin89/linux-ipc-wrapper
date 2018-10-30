CC=g++
CFLAGS=-std=c++17 -Wall -Wextra
DBGFLAGS=-ggdb 

BIN_PATH=bin/

TEST_SERVER_BIN=$(BIN_PATH)server
TEST_CLIENT_BIN=$(BIN_PATH)client

.PHONY : all clean

all: $(TEST_SERVER_BIN) $(TEST_CLIENT_BIN) 

$(TEST_SERVER_BIN): $(BIN_PATH)NamedPipeWrapper.o
	$(CC) $(CFLAGS) $(DBGFLAGS) ./tests/server.cpp -o $@ $^ 

$(TEST_CLIENT_BIN): $(BIN_PATH)NamedPipeWrapper.o
	$(CC) $(CFLAGS) $(DBGFLAGS) ./tests/client.cpp -o $@ $^ 

$(BIN_PATH)NamedPipeWrapper.o: ./src/NamedPipeWrapper.cpp 
	$(CC) $(CFLAGS) -c ./src/NamedPipeWrapper.cpp -o $@


clean:
	rm -f ./bin/*