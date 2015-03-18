#include "Client.h"
#include <thread>

using namespace std;

Client::Client(){}
Client::~Client(){}
void  testThread(Client * client);

bool Client::Connect(string serverPath, string & error){
	unsigned int connectPort = 0;
	size_t colonPos = serverPath.find(':');
	std::string hostPart;
	std::string portPart;

	if (colonPos != std::string::npos)
	{
		hostPart = serverPath.substr(0, colonPos);
		portPart = serverPath.substr(colonPos + 1);
		std::stringstream parser(portPart);
		int port = 0;
		if (parser >> port)
		{
			if (port > 0 && port < 65536){
				connectPort = port;
			}
			else
			{
				error = std::string("Error connecting: Connect port is not in the range 1 - 65536");
				return false;
			}
		}
		else
		{
			error = std::string("Error connecting: Connect port is not a number");
			return false;
		}
	}
	if (connectPort == 0){
		error = std::string("");
		return false;
	} 
	#ifdef WIN32
		netLink::init();
	#endif
		thread t1(testThread, this);
	socket = this->socketManager.newMsgPackSocket();


	try{
		socket->initAsTcpClient(hostPart, connectPort, true);
	}
	catch (netLink::Exception exc){
		std::cout << "error: " << exc.code << endl;
	}

	auto status = socket->getStatus();
	if (status == netLink::READY){

		socketManager.listen();
		netLink::MsgPackSocket& msgPackSocket = *static_cast<netLink::MsgPackSocket*>(socket.get());
		msgPackSocket << MsgPack__Factory(MapHeader(2));
		msgPackSocket << MsgPack::Factory("type");
		msgPackSocket << MsgPack::Factory("post");
		msgPackSocket << MsgPack::Factory("message");
		msgPackSocket << MsgPack::Factory("Hello World!");





		return true;
	}

	return false;
}

void  testThread(Client * client){
	while (client->socket->getStatus() != netLink::NOT_CONNECTED) {
	client->socketManager.listen();
	}
}


/*
bool Client::Connect(string serverPath, string & error){


	IPaddress ip;
	if (SDLNet_ResolveHost(&ip, hostPart.c_str(), connectPort) == -1) {
		error = std::string("Cannot resolve hostname of server: ").append(SDLNet_GetError());
		return false;
	}
	this->socket = SDLNet_TCP_Open(&ip);
	if (!socket) {
		error = std::string("Cannot connect to server: ").append(SDLNet_GetError());
		return false;
	}
	this->sendThread = SDL_CreateThread(sending, "SendingThread", this);
	this->receiveThread = SDL_CreateThread(recieving, "SendingThread", this);

	return true;
}

int sending(void *data)
{
	printf("sending thread\n");


	Client * thisClient = (Client *) data;

	auto mytest = string("a");
	int len = 1;
	if (SDLNet_TCP_Send(thisClient->socket, mytest.c_str(), len) < len)
	{
		fprintf(stderr, "SDLNet_TCP_Send: %s\n", SDLNet_GetError());
		exit(EXIT_FAILURE);
	}

	return 0;

}

int recieving(void *data)
{
	printf("recieving thread\n");

	Client * thisClient = (Client *)data;

	#define MAXLEN 1024
	int result;
	char msg[MAXLEN];

	result = SDLNet_TCP_Recv(thisClient->socket, msg, MAXLEN - 1);
	if (result > 0){
		msg[result] = 0;
		printf("Received: \"%s\"\n", msg);






	}
	
	return 0;
}

*/

