#pragma once
#include <SDL2/SDL_net.h>
#include <string>
#include <sstream>
#include <SDL2/SDL_thread.h>
#include <SDL2/SDL.h>
#include <iostream>
#include "deps/static/netlink/netLink.h"
#include <thread>

using namespace std;

class Client
{
private:
public:
	Client();
	~Client();
	bool Connect(string serverPath, string & error);
	std::shared_ptr<netLink::Socket> socket;
	netLink::SocketManager socketManager;


};

