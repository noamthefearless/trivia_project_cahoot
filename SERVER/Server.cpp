#include "Server.h"




/*
the C'tor: this function will create a new server
input: non
output: non
*/
Server::Server()
{
	m_database = new SqliteDatabase();
	m_handlerFactory = RequestHandlerFactory(LoginManager(m_database), RoomManager(), StatisticManager(m_database), GameManager(m_database), m_database);
	m_communicator = Communicator(&m_handlerFactory);
}


/*
the D'tor: this function will delete a server
input: non
output: non
*/
Server::~Server()
{
	delete m_database;
}

/// <summary>
/// This function runs the the server
/// </summary>
void Server::run()
{
	thread t(&Communicator::startHandleRequests, m_communicator);
	t.detach();
	string adminCommand = "";
	while (adminCommand != "EXIT")
	{
		getline(cin, adminCommand);
	}
}















