#include "web_socket_server.hpp"

#include <iostream>
#include <cstdlib>

#include <vector>
#include <list>
#include <map>
#include <algorithm>

class Player
{

};

int main(int argc, char *argv[], char *env[])
{
	WebSocketServer *server = new WebSocketServer();

	server->Init();

	return 0;
}
