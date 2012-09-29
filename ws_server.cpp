// For the Base64 encoding/decoding
#include "libb64/include/b64/encode.h"
#include "libb64/include/b64/decode.h"

// This is the for the sha1 of the key
#include "sha1.h"

#include <iostream>
#include <cstdlib>

// for threading
#include <pthread.h>

// For sockets
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

#define DEBUG

// This function creates the upgrade header for the server to send to a client which 
// will initiate the websocket.
std::string get_header(const std::string response_key)
{
	std::string websock_resp;
	websock_resp = "HTTP/1.1 101 Switching Protocols\r\n";
	websock_resp = websock_resp + "Upgrade: WebSocket\r\n";
	websock_resp = websock_resp + "Connection: Upgrade\r\n";
	websock_resp = websock_resp + "Sec-WebSocket-Origin: http://localhost\r\n";
	websock_resp = websock_resp + "Sec-WebSocket-Location: ws://localhost:9998/echo\r\n";
	websock_resp = websock_resp + "Sec-WebSocket-Accept: " + response_key + "\r\n";

	#ifdef DEBUG
	std::cout << "Websocket Server Response:" << std::endl << websock_resp << std::endl;
	#endif DEBUG

	return websock_resp;
}

// Creates the response key from the request key which is a magic UUID sha1'd
// with the original key and then b64'd back to the client.
std::string create_response_key(const std::string request_key)
{
	return "";
}

// Parses the header and returns the key which is send with the request. Assumes
// an HTTP websocket request.
std::string get_key_from_request_header(std::string request)
{

}

// the frames can have multiple sizes. But we will only be sending data of 126 bytes or less... for now.
struct frame
{
	uint8_t fin  : 1;
	uint8_t rsv1 : 1;
	uint8_t rsv2 : 1;
	uint8_t rsv3 : 1;
	uint8_t opcode : 4;

	
	uint8_t mask : 1;
	uint8_t payload_size : 7;
};

int main(int argc, char *argv[], char *env[])
{
	int listener_socket = 0, client_socket[8];

	// This is sha1 specific digest agregate that gets passed to each of the sha1 functions. see sha1.c
	SHA1_CTX context;

	get_header("");

	return 0;
}