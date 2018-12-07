CC=g++
CFLAGS=-std=c++17 -Wall -Wextra 
DBGFLAGS=-ggdb 
LINKER_FLAGS=-lrt

BIN_PATH=bin/

TEST_SERVER_BIN=$(BIN_PATH)server
TEST_CLIENT_BIN=$(BIN_PATH)client

.PHONY : all clean

all: $(TEST_SERVER_BIN) $(TEST_CLIENT_BIN) 

$(TEST_SERVER_BIN): $(BIN_PATH)MsgQueueWrapper.o ./tests/server.cpp
	$(CC) $(CFLAGS) $(DBGFLAGS) -o $@ $^   $(LINKER_FLAGS)

$(TEST_CLIENT_BIN): $(BIN_PATH)MsgQueueWrapper.o ./tests/client.cpp
	$(CC) $(CFLAGS) $(DBGFLAGS)  -o $@ $^  $(LINKER_FLAGS)

$(BIN_PATH)MsgQueueWrapper.o: ./src/MsgQueueWrapper.cpp 
	$(CC) $(CFLAGS) $(LINKER_FLAGS) -c ./src/MsgQueueWrapper.cpp -o $@


clean:
	rm -f ./bin/*