#include"Communicator.h"
#pragma comment(lib, "ws2_32.lib")



mutex clientMU;







/*
the C'tor: this function will create a new server
input: non
output: non
*/
Communicator::Communicator(RequestHandlerFactory* factory)
{
	WSADATA wsa_data = { };
	if (WSAStartup(MAKEWORD(2, 2), &wsa_data) != 0)
	{
		throw std::runtime_error("WSAStartup Failed");
	}
	m_serverSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (m_serverSocket == INVALID_SOCKET)
	{
		throw std::runtime_error("Unable to initialize socket!");
	}
	m_handlerFactory = factory;
}


/*
the D'tor: this function will delete a server
input: non
output: non
*/
Communicator::~Communicator()
{

}





/*
bindAndListen: this function will bind the port to the server socket and start to listen with it 
input: non
output: non
*/
void Communicator::bindAndListen()
{
	struct sockaddr_in sa = { 0 };

	sa.sin_port = htons(PORT); // port that server will listen for
	sa.sin_family = AF_INET;   // must be AF_INET
	sa.sin_addr.s_addr = INADDR_ANY;    // when there are few ip's for the machine. We will use always "INADDR_ANY"
	 //Connects between the socket and the configuration (port and etc..)
	if (bind(m_serverSocket, (struct sockaddr*)&sa, sizeof(sa)) == SOCKET_ERROR)
	{
		throw std::runtime_error("couldn't bind server socket");
	}
	// Start listening for incoming requests of clients
	if (listen(m_serverSocket, SOMAXCONN) == SOCKET_ERROR)
	{
		throw std::runtime_error("couldn't listen with server socket");
	}
}






/*
startHandleRequests: this function will accept new clients, and devide the sockets based on their code
input: non
output: non
*/
void Communicator::startHandleRequests()
{
	bindAndListen();
	while (true)
	{
		SOCKET client_socket = accept(m_serverSocket, NULL, NULL);
		if (client_socket == INVALID_SOCKET)
		{
			throw std::runtime_error("connection could not be established");
		}
		else
		{
			std::cout << "A wild client has appeard!" << std::endl;
			std::unique_lock<mutex> lock(clientMU);
			m_clients[client_socket] = nullptr;
			lock.unlock();
			thread t(&Communicator::handleNewClient, this, client_socket);// creati a new handler
			t.detach();
		}	
	}
}




/*
handleNewClient: this function will be handelling each client individually
input: the socket of the client
output: non
*/
void Communicator::handleNewClient(SOCKET sock)
{
	RequestInfo reqInfo;
	RequestResult reqRes;
	std::unique_lock<mutex> lock(clientMU);
	m_clients[sock] = m_handlerFactory->createLoginRequestHandler();
	lock.unlock();
	while (sock != INVALID_SOCKET)
	{
		try
		{
			reqInfo = extractRequestInfo(sock);//extracting the request info
		}
		catch (std::runtime_error err)
		{
			std::cout << err.what() << std::endl;

			reqInfo.requestCode = LEAVE_GAME;
			m_clients[sock] = m_clients[sock]->handleRequest(reqInfo).newHandler;

			reqInfo.requestCode = CLOSE_ROOM;//these 6 lines will handle connection dropped
			m_clients[sock] =  m_clients[sock]->handleRequest(reqInfo).newHandler;

			reqInfo.requestCode = LEAVE_ROOM;
			m_clients[sock] = m_clients[sock]->handleRequest(reqInfo).newHandler;

			reqInfo.requestCode = LOGOUT;
			m_clients[sock]->handleRequest(reqInfo);

			removeClient(sock);
			return;
		}
		lock.lock();
		reqInfo.receivalTime = std::time(nullptr);// taking the current time
		reqRes = m_clients[sock]->handleRequest(reqInfo);
		if (reqRes.newHandler != m_clients[sock])
		{
			delete m_clients[sock];
			m_clients[sock] = reqRes.newHandler;
		}
		lock.unlock();
		send(sock, (char*)reqRes.response.data(), reqRes.response.size(), NULL);
	}
}


/*
removeClient: this function will remove a client from the client list
input: the client's socket
output: non
*/
void Communicator::removeClient(SOCKET sock)
{
	std::unique_lock<mutex> lock(clientMU);
	closesocket(sock);
	delete	m_clients[sock];
	m_clients.erase(sock);// removing from the client list with a mutex
	lock.unlock();
}




/*
extractRequestInfo: this function will extract the request info from the socket itself
input: the socket
output: the RequestInfo struct
*/
RequestInfo Communicator::extractRequestInfo(SOCKET sock)
{
	RequestInfo reqInfo;
	unsigned char code = 0;
	vector<unsigned char> lengthBuff(4, 0);
	vector<unsigned char> dataBuff;
	int length = 0;
	if (recv(sock, (char*)&code, sizeof(unsigned char), NULL) <= 0)// first recieving the code
	{
		throw std::runtime_error("Code wasn't recived. connection dropped");
	}
	if (recv(sock, (char*)lengthBuff.data(), sizeof(int), NULL) <= 0)// getting length
	{
		throw std::runtime_error("length wasn't recived. connection dropped");
	}
	reqInfo.requestCode = code;
	try
	{
		length = JsonRequestPacketDeserializer::DeserializeRequsetLength(lengthBuff);//deserializing length
	}
	catch (std::runtime_error err)
	{
		throw std::runtime_error("length wasn't valid. connection dropped");
	}
	dataBuff.resize(length);
	if (recv(sock, (char*)dataBuff.data(), length, NULL) <= 0)//getting data
	{
		throw std::runtime_error("length wasn't recived. connection dropped");
	}
	reqInfo.buffer = dataBuff;
	return reqInfo;


}
















