// For the Base64 encoding/decoding
#include "libb64/include/b64/encode.h"
#include "libb64/include/b64/decode.h"

// This is the for the sha1 of the key
#include "sha1.h"

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
#include <signal.h>

// gives the data size -1 which would account for a null byte. The data
// is being read as binary data so there will be no null byte.
#define NONULL(size) (size-1)

// The debugging flag. comment out to get rid of the debugging output.
#define DEBUG

// This will be the handler to clean up the server when it is finished.
void sigint_handler(int signal)
{
	exit(1);
	return;
}

// THis is the dataframe diagram directly from the RFC 6455
// +-+-+-+-+-------+-+-------------+-------------------------------+
//  0                   1                   2                   3
//  0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
// +-+-+-+-+-------+-+-------------+-------------------------------+
// |F|R|R|R| opcode|M| Payload len |    Extended payload length    |
// |I|S|S|S|  (4)  |A|     (7)     |             (16/64)           |
// |N|V|V|V|       |S|             |   (if payload len==126/127)   |
// | |1|2|3|       |K|             |                               |
// +-+-+-+-+-------+-+-------------+ - - - - - - - - - - - - - - - +
// |     Extended payload length continued, if payload len == 127  |
// + - - - - - - - - - - - - - - - +-------------------------------+
// |                               | Masking-key, if MASK set to 1 |
// +-------------------------------+-------------------------------+
// | Masking-key (continued)       |          Payload Data         |
// +-------------------------------- - - - - - - - - - - - - - - - +
// :                     Payload Data continued ...                :
// + - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - +
// |                     Payload Data continued ...                |
// +---------------------------------------------------------------+

const std::string UUID = "258EAFA5-E914-47DA-95CA-C5AB0DC85B11";

// the frames can have multiple sizes. But we will only be sending data of 126 bytes or less... for now.
struct frame
{
	// byte 1
	uint8_t fin  : 1;
	uint8_t rsv1 : 1;
	uint8_t rsv2 : 1;
	uint8_t rsv3 : 1;
	uint8_t opcode : 4;

	// byte 2
	uint8_t maskset : 1;
	uint8_t payload_size : 7;

	// byte 3 -- byte 3 can be used to represent the size of the data being passed if the payload size is >= 126
	// For this application I am going to assume that all the data being passed back and forth is less than 126 in size.
	// If that turns out to not be the case then I can change it later.

	// instead of having a size byte we will have the masking bytes.
	uint8_t mask[NONULL(4)];

	// the payload as its max size for this server
	uint8_t data[NONULL(126)];
};

// This function creates the upgrade header for the server to send to a client which 
// will initiate the websocket.
std::string get_header(const std::string response_key)
{
	std::string websock_resp;
	websock_resp = "HTTP/1.1 101 Switching Protocols\r\n";
	websock_resp += "Upgrade: WebSocket\r\n";
	websock_resp += "Connection: Upgrade\r\n";
	websock_resp += "Sec-WebSocket-Origin: http://localhost\r\n";
	websock_resp += "Sec-WebSocket-Location: ws://localhost:9998/echo\r\n";
	websock_resp += "Sec-WebSocket-Accept: " + response_key + "\r\n";

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

// clears the whitespace from either side of a string
std::string chomp_str(std::string src)
{
	std::string bald_string = src;

	// remove whitespace from the front of the string
	int i = 0;
	char ch = bald_string[i];
	while(ch == ' ' || ch == '\n' || ch == '\r' || ch == '\t')
	{
		bald_string.erase(i, 1);
		ch = bald_string[i];
	}

	// remove whitespace from the end of the string
	i = bald_string.length()-1;
	ch = bald_string[i];
	while(ch == ' ' || ch == '\r' || ch == '\n' || ch == '\t')
	{
		bald_string.erase(i, 1);
		ch = bald_string[--i];
	}

	return bald_string;
}

// splits a string on a token into many strings up to 128
std::vector<std::string> split_str(std::string src, char token)
{
	std::vector<std::string> strings;

	int substr_size = 1;
	for(int i = 0; i < (int)src.length(); i++)
	{
		if(src[i] == token)
		{
			std::string temp = src.substr((i-substr_size+1), substr_size);
			strings.push_back(temp);
			substr_size = 1;
		}
		substr_size += 1;
	}

	return strings;
}

// Parses the header and returns the key which is send with the request. Assumes
// an HTTP websocket request.
std::string get_key_from_request_header(std::string request)
{
	std::string key;

	std::vector<std::string> header_lines = split_str(request, '\n');
	std::vector<std::string>::iterator it;

	for(it = header_lines.begin(); it != header_lines.end(); it++)
	{
		#ifdef DEBUG
		std::cout << "SPLIT LINE: '" << (*it) << "'" << std::endl;
		#endif DEBUG
	}

	return key;
}

void digest_to_hex(const uint8_t digest[SHA1_DIGEST_SIZE], char *output)
{
    int i,j;
    char *c = output;
    
    for (i = 0; i < SHA1_DIGEST_SIZE/4; i++) {
        for (j = 0; j < 4; j++) {
            sprintf(c,"%02x", digest[i*4+j]);
            c += 2;
        }
    }
    // *(c) = '\0';
}

int main(int argc, char *argv[], char *env[])
{
	signal(SIGINT, &sigint_handler);

	int listener_socket = 0, client_socket[8];
	struct sockaddr_in serv_addr;

	char buffer[512];

	// time_t ticks;

	// set up the listener socket
	listener_socket = socket(AF_INET, SOCK_STREAM, 0);

	// make sure our data structures are clean
	memset(&serv_addr, '0', sizeof(serv_addr));

	// set up the serv_addr struct. THe port that we will be connecting on is 9998
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	serv_addr.sin_port = htons(9998);

	int error = bind(listener_socket, (struct sockaddr*)&serv_addr, sizeof(serv_addr));

	if(error != 0)
	{
		std::cout << "ERROR?: " << error << std::endl;
		exit(1);
	}

	// have a queue of 10 connections
	listen(listener_socket, 10);

	int socket_count = 0;

	while(1)
	{
		memset(buffer, '0', sizeof(buffer));
		client_socket[socket_count] = accept(listener_socket, (struct sockaddr*)NULL, NULL);

		// ticks = time(NULL);
		recv(client_socket[socket_count], buffer, strlen(buffer), 0);

		#ifdef DEBUG
		std::cout << "RECEIVED: " << std::endl << buffer << std::endl;
		#endif DEBUG

		std::string sbuffer = std::string(buffer);
		std::vector<std::string> headers = split_str(sbuffer, '\n');
		std::vector<std::string>::iterator it;

		std::cout << "headers: " << std::endl;
		for(it = headers.begin(); it != headers.end(); it++)
		{
			std::string header = chomp_str(*it);

			if(header.substr(0, 19) == "Sec-WebSocket-Key: ")
			{
				std::string key = chomp_str(header.substr(19, 100));
				std::cout << "KEY: " << key << std::endl;

				std::string new_key = key+UUID;
				std::cout << "new key: " << new_key << std::endl;

				uint8_t digest[SHA1_DIGEST_SIZE];

				// This is sha1 specific digest agregate that gets passed to each of the sha1 functions. see sha1.c
				SHA1_CTX context;
				SHA1_Init(&context);
				SHA1_Update(&context, (uint8_t*)new_key.c_str(), new_key.length());
				SHA1_Final(&context, digest);

				base64::encoder E;
				char b64[41];

				digest_to_hex(digest, b64);

				std::cout << "digest: " << b64 << " -- " << digest << std::endl;
				char b64string[128];
				memset(b64string, '\0', sizeof(b64string));

				// 40uDlqO8fWbqWt3fzXi0cKy13a

				//vXy9QC4ObRB4g6nOHoi0Hj8CV
				// TBAn5AN3x1jxf2tysYzsY5HMYTQ=
				// xy5XoXMQkMsiyB42z2e4Gu4Dm1w=

				E.encode((char*)digest, SHA1_DIGEST_SIZE+1, b64string);

				std::cout << "b64 stream: " << b64string << std::endl;
				break;
			}
		}
	}

	return 0;
}