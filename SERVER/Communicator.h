#pragma once
#pragma comment(lib, "ws2_32.lib")
#include<WinSock2.h>
#include <Windows.h>
#include <stdexcept>
#include <map>
#include <vector>
#include"IRequestHandler.h"
#include "LoginRequestHandler.h"
#include <thread>
#include<string>
#include<iostream>
#include <mutex>
using std::mutex;
using std::vector;
using std::string;
using std::thread;
using std::cout;
using std::map;
#define PORT 9090
class Communicator
{
public:
	Communicator() = default;
	Communicator(RequestHandlerFactory* factory);
	~Communicator();
	void startHandleRequests();






private:
	SOCKET m_serverSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	map<SOCKET, IRequestHandler*> m_clients;
	RequestHandlerFactory* m_handlerFactory;
	void bindAndListen();
	void handleNewClient(SOCKET sock);
	RequestInfo extractRequestInfo(SOCKET sock);
	void removeClient(SOCKET sock);



};

