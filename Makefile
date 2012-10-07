# This is here because the creator of libb64 puts the buffer size in the make file.
BUFFERSIZE=16777216
FLAGS = -DBUFFERSIZE=$(BUFFERSIZE)

SRC_DIR = ./src/

# This is for the threading library posix thread
FLAGS += -lpthread 

# For showing all the warnings
FLAGS += -Wall

web_socket_server: main.o web_socket_server.o list.o
	g++ -o web_socket_server main.o web_socket_server.o sha1.o base64.o list.o

main.o: $(SRC_DIR)main.cpp
	g++ -c $(SRC_DIR)main.cpp

web_socket_server.o: $(SRC_DIR)web_socket_server.cpp $(SRC_DIR)web_socket_server.hpp sha1.o base64.o
	g++ -c $(SRC_DIR)web_socket_server.cpp

list.o: $(SRC_DIR)list.hpp $(SRC_DIR)list.cpp
	g++ -c $(SRC_DIR)list.cpp

# This is the first iteration of the server. It is not longer going to be used for the game but can still
# be built and used for testing. However, some of the rules for this build will still be used.
ws_server: ws_server.o sha1.o base64.o
	g++ -o ws_server ws_server.o sha1.o base64.o $(FLAGS)

ws_server.o: $(SRC_DIR)ws_server.cpp
	g++ -c $(SRC_DIR)ws_server.cpp

sha1.o: $(SRC_DIR)sha1.c $(SRC_DIR)sha1.h
	g++ -c $(SRC_DIR)sha1.c

base64.o:
	g++ -c $(SRC_DIR)base64.cpp

clean:
	rm -f ws_server, *.o