#pragma once
#include "RoomRequestsHandler.h"
#include "IRequestHandler.h"
class RequestHandlerFactory;


class RoomAdminRequestHandler : public IRequestHandler
{
public:
	RoomAdminRequestHandler() = default;
	RoomAdminRequestHandler(Room* myRoom, LoggedUser myUser, RequestHandlerFactory* myFactory);
	~RoomAdminRequestHandler();
	virtual bool isRequestRelevant(RequestInfo reqInfo);
	virtual RequestResult handleRequest(RequestInfo reqInfo);



private:
	Room* m_room;
	LoggedUser m_user;
	RequestHandlerFactory* m_handlerFactory;
};

