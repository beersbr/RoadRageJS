#include "web_socket_server.hpp"

WebSocketServer::WebSocketServer()
{
	this->errnum = 0;

	client_socket = NULL;
	listener_socket = -1;

	// Eventually will be fetched from a config file.
	listener_portno = 9998;

	// zero out the sockaddr_in struct
	memset((void*)&serv_addr, '0', sizeof(serv_addr));
}

WebSocketServer::WebSocketServer(int portno)
{
	client_socket = NULL;
	listener_socket = -1;
	listener_portno = portno;

	memset((void*)&serv_addr, '0', sizeof(serv_addr));
}

WebSocketServer::~WebSocketServer()
{
}

int WebSocketServer::Init()
{
	// set up the listener
	listener_socket = socket(AF_INET, SOCK_STREAM, 0);

	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	serv_addr.sin_port = htons(listener_portno);

	errno = bind(listener_socket, (struct sockaddr*)&serv_addr, sizeof(serv_addr));
}

int WebSocketServer::Listen(int queue_size)
{
	// spin off the listener thread
}

void* WebSocketServer::_listen(void *args)
{
	int queue_size = *((int*)args);

	listen(listener_socket, queue_size);
	std::cout << "Listener Thread (" << listener_portno << "): " << pthread_self() << std::endl;
}

int WebSocketServer::GetLastError()
{
	std::cout << "Last Socket Error: " << this->errnum << std::endl;
	return (this->errnum);
}