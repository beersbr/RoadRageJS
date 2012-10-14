#ifndef WEB_SOCKET_SERVER_HEADER
#define WEB_SOCKET_SERVER_HEADER

// This is the for the sha1 of the key
#include "base64.h"
#include "sha1.h"

// Standard includes
#include <iostream>
#include <iomanip>
#include <cstdlib>

// STL stuff
#include <vector>
#include <list>
#include <map>
#include <algorithm>

// for threading
#include <pthread.h>

// For sockets, timeing and such
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <time.h>
#include <signal.h>
#include <fcntl.h>

#include "helpers.hpp"

#define ARRAYSIZE(size) (size-1)

#define READBUFFER 512

// the magic UUID for the server websocket response
const std::string UUID = "258EAFA5-E914-47DA-95CA-C5AB0DC85B11";

// WebsocketFrame is a match of the standard from the RFC listed in the readme.md. We will be using a static size 
struct WSFrame
{
	// Because we are splitting bytes we need to have them backward as that is how they will be read in.
	unsigned char opcode : 4;
	unsigned char rsv3 : 1;
	unsigned char rsv2 : 1;
	unsigned char rsv1 : 1;
	unsigned char fin  : 1;
	
	// byte 2
	unsigned char payload_size : 7;
	unsigned char maskset : 1;

	union size_buffer{
		struct sz_125{
			unsigned char mask[4];
			unsigned char data[ARRAYSIZE(126)];
		};
		struct sz_126{
			unsigned char size_126[2];
			unsigned char mask[4];
			unsigned char data[ARRAYSIZE(65535)];
		};
		struct sz_127{
			unsigned char size_127[8];
			unsigned char mask[4];
			unsigned char data1[ARRAYSIZE(4294967295)];
			unsigned char data2[ARRAYSIZE(4294967295)];
		};
	};
};

struct WSFrameHeader{
	unsigned char opcode : 4;
	unsigned char rsv3 : 1;
	unsigned char rsv2 : 1;
	unsigned char rsv1 : 1;
	unsigned char fin  : 1;

	unsigned char payload_size : 7;
	unsigned char maskset : 1;
};


// This is the wrapper for the frame.
class WebSocketFrame
{
public:
	WebSocketFrame();
	~WebSocketFrame();

private:
	struct WSFrame frame;

};

void SigHandler(int signal);

class WebSocketServer
{
public:

	int Init();
	int Listen(int queue_size = 10);
	int GetClientSocket(void* (*func)(void*));

	char* ReadSocketBuffer(char *dataFrameBuffer, int sz_buf);
	char* WriteSocketBuffer(const char* buffer, int* sz_buf);

	int GetLastError();

	static int server_running; // TODO: this is getting removed...

	static WebSocketServer* Instance();

	~WebSocketServer();

private:

	WebSocketServer();

	std::string getRequestKey(const std::string request_header);
	std::string createHeader(const std::string response_key);
	std::string createResponseKey(const std::string request_key);

	struct sockaddr_in serv_addr;

	int *client_socket;
	int listener_socket;
	int listener_portno;

	int errnum;

	char buffer[READBUFFER];

	static WebSocketServer* instance;
};

#endif