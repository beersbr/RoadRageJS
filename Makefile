
# This is the static library for the b64 decoder/encoder
LIB_LIBB64=libb64/src/libb64.a

# This is here because the creator of libb64 puts the buffer size in the make file.
BUFFERSIZE=16777216
FLAGS = -DBUFFERSIZE=$(BUFFERSIZE)

# This is for the threading library posix thread
FLAGS += -lpthread 

# For showing all the warnings
FLAGS += -Wall

ws_server: ws_server.cpp sha1.o base64.o
	g++ -o ws_server ws_server.cpp sha1.o base64.o $(FLAGS)

sha1.o: sha1.c sha1.h
	g++ -c sha1.c

base64.o:
	g++ -c base64.cpp

clean:
	rm -f ws_server, *.o