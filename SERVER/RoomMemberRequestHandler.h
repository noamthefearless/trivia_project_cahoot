#pragma once

#include "RoomRequestsHandler.h"
#include "IRequestHandler.h"
class RequestHandlerFactory;


class RoomMemberRequestHandler : public IRequestHandler
{
public:
	RoomMemberRequestHandler() = default;
	RoomMemberRequestHandler(Room* myRoom, LoggedUser myUser, RequestHandlerFactory* myFactory);
	~RoomMemberRequestHandler();
	virtual bool isRequestRelevant(RequestInfo reqInfo);
	virtual RequestResult handleRequest(RequestInfo reqInfo);



private:
	Room* m_room;
	LoggedUser m_user; 
	RequestHandlerFactory* m_handlerFactory;

};

