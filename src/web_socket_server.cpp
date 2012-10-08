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

	server_running = false;
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
	server_running = true;

	listen(listener_socket, queue_size);

	while(server_running)
	{
		client_socket = (int*)malloc(sizeof(int));
		(*client_socket) = accept(listener_socket, (struct sockaddr*)NULL, NULL);

		memset(buffer, '0', sizeof(buffer));

		recv(*client_socket, buffer, strlen(buffer), 0);

		std::string sbuffer = std::string(buffer);
		std::vector<std::string> headers;

		// TODO: Get the header
		// TODO: Get the key
		// TODO: create the response key
		// TODO: Send the response header
		// TODO: Start the new thread with new good connection
	}

	close(listener_socket);
}

int WebSocketServer::GetLastError()
{
	std::cout << "Last Socket Error: " << this->errnum << std::endl;
	return (this->errnum);
}