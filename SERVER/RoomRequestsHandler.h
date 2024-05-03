#pragma once
#include "IRequestHandler.h"
#include "RequestHandlerFactory.h"
#include "RoomAdminRequestHandler.h"
#include "RoomMemberRequestHandler.h"



class RoomRequestsHandler 
{
public:
	static RequestResult handleRequest(RequestInfo reqInfo, Room* room, LoggedUser user, RequestHandlerFactory* handlerFactory, IRequestHandler* Sender);

	static RequestResult leaveRoom(RequestInfo reqInfo, Room* room, LoggedUser user, RequestHandlerFactory* handlerFactory, IRequestHandler* Sender);
	static RequestResult closeRoom(RequestInfo reqInfo, Room* room, LoggedUser user, RequestHandlerFactory* handlerFactory, IRequestHandler* Sender);
	static RequestResult startGame(RequestInfo reqInfo, Room* room, LoggedUser user, RequestHandlerFactory* handlerFactory, IRequestHandler* Sender);
	static RequestResult getRoomState(RequestInfo reqInfo, Room* room, LoggedUser user, RequestHandlerFactory* handlerFactory, IRequestHandler* Sender);



};

