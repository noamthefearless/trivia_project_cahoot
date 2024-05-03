#pragma once
#include "IRequestHandler.h"
#include "RequestHandlerFactory.h"
#include "IDatabase.h"

class RequestHandlerFactory;


class MenuRequestHandler : public IRequestHandler
{
public:
	MenuRequestHandler() = default;
	MenuRequestHandler(LoggedUser user, RequestHandlerFactory* factory);
	~MenuRequestHandler();
	virtual bool isRequestRelevant(RequestInfo reqInfo);
	virtual RequestResult handleRequest(RequestInfo reqInfo);






private:
	RequestResult logout(RequestInfo reqInfo);
	RequestResult getRooms(RequestInfo reqInfo);
	RequestResult getPlayersInRoom(RequestInfo reqInfo);
	RequestResult getPersonalStats(RequestInfo reqInfo);
	RequestResult getHighScore(RequestInfo reqInfo);
	RequestResult joinRoom(RequestInfo reqInfo);
	RequestResult createRoom(RequestInfo reqInfo);
	RoomData getCreateRoomRequestRoomData(CreateRoomRequest req);

	LoggedUser m_user;
	RequestHandlerFactory* m_handlerFactory;
};

