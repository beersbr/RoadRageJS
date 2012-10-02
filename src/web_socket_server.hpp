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

#define ARRAYSIZE(size) (size-1)

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

	// Mask
	unsigned char mask[ARRAYSIZE(4)];

	// the payload as its max size for this server
	unsigned char data[ARRAYSIZE(126)];
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

class WebSocketServer
{
public:
	WebSocketServer();
	WebSocketServer(int portno);
	~WebSocketServer();

	int Init();
	int Listen(int queue_size);
	int GetClientSocket();

	int GetLastError();

private:

	void* _listen(void* args);

	struct sockaddr_in serv_addr;

	int *client_socket;
	int listener_socket;
	int listener_portno;

	int errnum;

};

#endif