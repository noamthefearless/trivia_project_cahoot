#include "RoomAdminRequestHandler.h"




/*
the C'tor: this function will create a new RoomAdminRequestHandler
input: the room, the user, and the factory ptr
output: non
*/
RoomAdminRequestHandler::RoomAdminRequestHandler(Room* myRoom, LoggedUser myUser, RequestHandlerFactory* myFactory)
{
	m_handlerFactory = myFactory;
	m_room = myRoom;
	m_user = myUser;
}


/*
the D'tor: this function will delete a RoomAdminRequestHandler
input: non
output: non
*/
RoomAdminRequestHandler::~RoomAdminRequestHandler()
{

}


/*
isRequestRelevant: this function will check if the code in the request matchs one of the  codes
input: the request info struct
output: a bool saying if the code is valid or not
*/
bool RoomAdminRequestHandler::isRequestRelevant(RequestInfo reqInfo)
{
	if (reqInfo.requestCode == GET_ROOM_STATE || reqInfo.requestCode == CLOSE_ROOM || reqInfo.requestCode == START_GAME)
	{
		return true;
	}
	return false;
}




RequestResult RoomAdminRequestHandler::handleRequest(RequestInfo reqInfo)
{
	RequestResult result;
	ErrorResponse err;
	RequestResult errCase;
	try
	{
		if (isRequestRelevant(reqInfo))
		{
			return RoomRequestsHandler::handleRequest(reqInfo, m_room, m_user, m_handlerFactory, this);
		}
		else
		{
			throw std::runtime_error("invalid code");
		}
	}
	catch (...)
	{
		err.message = "Message is invalid! please make sure that the code, length, and data are all valid before sending";
		errCase.response = JsonResponsePacketSerializer::serializeResponse(err);
		errCase.newHandler = this;
		return errCase;
	}

	return result;
}




