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

std::string WebSocketServer::createHeader(const std::string response_key)
{
	std::string websock_resp;
	websock_resp = "HTTP/1.1 101 Switching Protocols\r\n";
	websock_resp += "Upgrade: WebSocket\r\n";
	websock_resp += "Connection: Upgrade\r\n";
	websock_resp += "Sec-WebSocket-Origin: http://localhost\r\n";
	websock_resp += "Sec-WebSocket-Location: ws://localhost:9998/echo\r\n";
	websock_resp += "Sec-WebSocket-Accept: " + response_key + "\r\n";
	websock_resp += "\r\n";

	return websock_resp;
}

std::string createResponseKey(const std::string request_key)
{
	// initalize the SH1 hasher
	SHA1_CTX context;
	uint8_t digest[SHA1_DIGEST_SIZE];

	// std::cout << "Recieved Key: " << request_key << std::endl;
	std::string response_key = request_key + UUID;
	// std::cout << "Response Key: " << response_key << std::endl;

	SHA1_Init(&context);
	SHA1_Update(&context, (uint8_t*)response_key.c_str(), response_key.length());
	SHA1_Final(&context, digest);

	response_key = base64_encode(digest, sizeof(digest));

	// std::cout << "Response Key: " << response_key << std::endl;

	return response_key;
}

int WebSocketServer::GetLastError()
{
	std::cout << "Last Socket Error: " << this->errnum << std::endl;
	return (this->errnum);
}