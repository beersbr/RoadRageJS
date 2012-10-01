// For the Base64 encoding/decoding
// #include "libb64/include/b64/encode.h"
// #include "libb64/include/b64/decode.h"

// This is the for the sha1 of the key
#include "base64.h"
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
// #define DEBUG

const std::string UUID = "258EAFA5-E914-47DA-95CA-C5AB0DC85B11";

// the frames can have multiple sizes. But we will only be sending data of 126 bytes or less... for now.
// keep in mind the bits come in backwards with the lowest bit last.
struct frame
{
	// byte 1
	unsigned char opcode : 4;
	unsigned char rsv3 : 1;
	unsigned char rsv2 : 1;
	unsigned char rsv1 : 1;
	unsigned char fin  : 1;
	
	// byte 2
	unsigned char payload_size : 7;
	unsigned char maskset : 1;

	// instead of having a size byte we will have the masking bytes.
	unsigned char mask[NONULL(4)];

	// the payload as its max size for this server
	unsigned char data[NONULL(126)];
};

// This function is called after the server has exchanged keys
static void* worker(void *arg)
{
	std::cout << "Started thread: " << pthread_self() << std::endl;

	int client_socket = *((int *) arg);
	free(arg);

	pthread_detach(pthread_self());

	char buffer[2048];
	int recv_size = 0;

	struct frame* dataframe = (struct frame*)malloc(sizeof(struct frame) );

	while(1)
	{
		memset(buffer, '\0', 2048);
		recv_size = recv(client_socket, buffer, 2048, 0);

		memcpy((void*)dataframe, buffer, sizeof(struct frame));
		std::cout << "fin : " << (unsigned int)dataframe->fin     << std::endl;
		std::cout << "rsv1: " << (int)dataframe->rsv1    << std::endl;
		std::cout << "rsv2: " << (int)dataframe->rsv2    << std::endl;
		std::cout << "rsv3: " << (int)dataframe->rsv3    << std::endl;
		std::cout << "opco: " << (int)dataframe->opcode  << std::endl;

		std::cout << "mask: " << (unsigned int)dataframe->maskset << std::endl;
		std::cout << "size: " << (unsigned short)dataframe->payload_size << std::endl;

		std::cout << "mas0: " << (int)dataframe->mask[0] << std::endl;
		std::cout << "mas1: " << (int)dataframe->mask[1] << std::endl;
		std::cout << "mas2: " << (int)dataframe->mask[2] << std::endl;
		std::cout << "mas3: " << (int)dataframe->mask[3] << std::endl;

		std::cout << "data: " << (char*)(dataframe->data) << std::endl;

		for(int i = 0; i < (int)dataframe->payload_size; i++)
		{
			std::cout << (char)((unsigned char)dataframe->data[i] ^ (unsigned char)dataframe->mask[(i+3)%4]);
		}
		std::cout << std::endl;

		break;
	}

	close(client_socket);
	return(NULL);
}

std::string get_header(const std::string response_key);
std::string create_response_key(const std::string request_key);
std::string chomp_str(std::string src);
std::vector<std::string> split_str(std::string src, char token);
std::string get_key_from_request_header(std::string request);
void digest_to_hex(const uint8_t digest[SHA1_DIGEST_SIZE], char *output);

/**************************************************************************************
Main function
**************************************************************************************/

int main(int argc, char *argv[], char *env[])
{
	int listener_socket = 0;
	int* client_socket_ptr = NULL;

	struct sockaddr_in serv_addr;

	pthread_t thread_id;

	char buffer[512];

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

	while(1)
	{
		client_socket_ptr = (int*)malloc(sizeof(int));
		memset(buffer, '0', sizeof(buffer));
		*client_socket_ptr = accept(listener_socket, (struct sockaddr*)NULL, NULL);

		recv(*client_socket_ptr, buffer, strlen(buffer), 0);

		#ifdef DEBUG
		std::cout << "RECEIVED: " << std::endl << buffer << std::endl;
		#endif DEBUG

		std::string sbuffer = std::string(buffer);
		std::vector<std::string> headers = split_str(sbuffer, '\n');
		std::vector<std::string>::iterator it;

		// search through the header and look for the header field that contains the key
		for(it = headers.begin(); it != headers.end(); it++)
		{
			std::string header = chomp_str(*it);

			if(header.substr(0, 19) == "Sec-WebSocket-Key: ")
			{
				// parse out the request key
				std::string key = chomp_str(header.substr(19, 100));

				std::string b64key = create_response_key(key);
				std::string new_header = get_header(b64key);

				#ifdef DEBUG
				std::cout << new_header << std::endl;
				#endif DEBUG

				send(*client_socket_ptr, new_header.c_str(), new_header.length(), 0);

				pthread_create(&thread_id, NULL, &worker, client_socket_ptr);

				break;
			}
		}
	}

	close(listener_socket);

	return 0;
}

/**************************************************************************************
functions definitions
**************************************************************************************/

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
	websock_resp += "\r\n";

	#ifdef DEBUG
	std::cout << "Websocket Server Response:" << std::endl << websock_resp << std::endl;
	#endif DEBUG

	return websock_resp;
}

// Creates the response key from the request key which is a magic UUID sha1'd
// with the original key and then b64'd back to the client.
std::string create_response_key(const std::string request_key)
{
	// initalize the SH1 hasher
	SHA1_CTX context;
	uint8_t digest[SHA1_DIGEST_SIZE];

	std::cout << "Recieved Key: " << request_key << std::endl;
	std::string response_key = request_key + UUID;
	std::cout << "Response Key: " << response_key << std::endl;

	SHA1_Init(&context);
	SHA1_Update(&context, (uint8_t*)response_key.c_str(), response_key.length());
	SHA1_Final(&context, digest);

	response_key = base64_encode(digest, sizeof(digest));

	#ifdef DEBUG
	char b64[41];
	digest_to_hex(digest, b64);
	std::cout << "digest: " << b64 << " -- " << digest << std::endl;
	#endif DEBUG

	std::cout << "Response Key: " << response_key << std::endl;

	return response_key;
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
}
