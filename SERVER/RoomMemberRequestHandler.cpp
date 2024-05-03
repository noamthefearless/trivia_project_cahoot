#include "RoomMemberRequestHandler.h"





/*
the C'tor: this function will create a new RoomMemberRequestHandler
input: the room, the user, and the factory ptr
output: non
*/
RoomMemberRequestHandler::RoomMemberRequestHandler(Room* myRoom, LoggedUser myUser, RequestHandlerFactory* myFactory)
{
	m_room = myRoom;
	m_user = myUser;
	m_handlerFactory = myFactory;
}


/*
the D'tor: this function will delete a RoomMemberRequestHandler
input: non
output: non
*/
RoomMemberRequestHandler::~RoomMemberRequestHandler()
{

}


/*
isRequestRelevant: this function will check if the code in the request matchs one of the  codes
input: the request info struct
output: a bool saying if the code is valid or not
*/
bool RoomMemberRequestHandler::isRequestRelevant(RequestInfo reqInfo)
{
	if (reqInfo.requestCode == GET_ROOM_STATE || reqInfo.requestCode == LEAVE_ROOM)
	{
		return true;
	}
	return false;
}



RequestResult RoomMemberRequestHandler::handleRequest(RequestInfo reqInfo)
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















