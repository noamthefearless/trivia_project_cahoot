#pragma once
#include <string>
#include "Communicator.h"
#include "RequestHandlerFactory.h"
#include "IDatabase.h"
#include <thread>
using std::string;
#include<iostream>
using std::getline;
using std::cin;
using std::thread;

class Server
{
public:
	Server();
	~Server();
	void run();






private:

	Communicator m_communicator;
	IDatabase* m_database;
	RequestHandlerFactory m_handlerFactory;
};





















