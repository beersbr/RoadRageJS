#include "web_socket_server.hpp"

#include "list.hpp"
#include "entity.hpp"

#include <iostream>
#include <cstdlib>

#include <vector>
#include <list>
#include <map>
#include <algorithm>

int main(int argc, char *argv[], char *env[])
{
	WebSocketServer *server = new WebSocketServer();

	server->Init();

	return 0;
}
