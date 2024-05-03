#include "RoomRequestsHandler.h"



/*
handle request: thus function will identify the request type, and then handle it
input: the request info, the room and factory ptr, and the sender IRequestHandler ptr 
output: the request result
*/
RequestResult RoomRequestsHandler::handleRequest(RequestInfo reqInfo, Room* room, LoggedUser user, RequestHandlerFactory* handlerFactory, IRequestHandler* Sender)
{
	RequestResult result;
	ErrorResponse err;
	RequestResult errCase;

	if (reqInfo.requestCode == LEAVE_ROOM)//calling the right function
	{
		return RoomRequestsHandler::leaveRoom(reqInfo, room, user, handlerFactory, Sender);
	}
	else if (reqInfo.requestCode == GET_ROOM_STATE)
	{
		return RoomRequestsHandler::getRoomState(reqInfo, room, user, handlerFactory, Sender);
	}
	else if (reqInfo.requestCode == CLOSE_ROOM)
	{
		return RoomRequestsHandler::closeRoom(reqInfo, room, user, handlerFactory, Sender);
	}
	else if (reqInfo.requestCode == START_GAME)
	{
		return RoomRequestsHandler::startGame(reqInfo, room, user, handlerFactory, Sender);
	}
	else if (reqInfo.requestCode == GET_ROOM_STATE)
	{
		return RoomRequestsHandler::getRoomState(reqInfo, room, user, handlerFactory, Sender);
	}
	return RequestResult();
}

/*
leaveRoom: this function will leave a room as a member
input: the request info, the room and factory ptr, and the sender IRequestHandler ptr
output: the request result
*/
RequestResult RoomRequestsHandler::leaveRoom(RequestInfo reqInfo, Room* room, LoggedUser user, RequestHandlerFactory* handlerFactory, IRequestHandler* Sender)
{
	RequestResult result;
	LeaveRoomResponse leaveRoomResponse;
	if (room->isAdminStillInRoom() || room->getAllUsers().size() != 1)
	{
		room->removeUser(user);
	}
	else
	{
		handlerFactory->getRoomManager().deleteRoom(room->getRoomData().id);//deleting room
	}

	leaveRoomResponse.status = STATUS_POS;
	result.response = JsonResponsePacketSerializer::serializeResponse(leaveRoomResponse);
	result.newHandler = handlerFactory->createMenuRequestHandler(user);

	return result;
}

/*
getRoomState: this function will get the current state of the room
input: the request info, the room and factory ptr, and the sender IRequestHandler ptr
output: the request result
*/
RequestResult RoomRequestsHandler::getRoomState(RequestInfo reqInfo, Room* room, LoggedUser user, RequestHandlerFactory* handlerFactory, IRequestHandler* Sender)
{
	RequestResult result;
	GetRoomStateResponse getRoomStateResponse;
	if (room->isAdminStillInRoom() || room->getRoomData().isActive == ACTIVE)
	{
		getRoomStateResponse.status = STATUS_POS;
		getRoomStateResponse.hasGameBegun = room->getRoomData().isActive;

		getRoomStateResponse.players = room->getAllUsers();

		getRoomStateResponse.questionCount = room->getRoomData().numOfQuestionsInGame;

		getRoomStateResponse.answerTimeout = room->getRoomData().timePerQuestion;
	}
	else
	{
		getRoomStateResponse.status = STATUS_NEG;
	}
	if (room->getRoomData().isActive == ACTIVE)
	{
		result.newHandler = handlerFactory->createGameRequestHandler(user, handlerFactory->getGameManager().getGame(room->getRoomData().gameId));
		if (room->getAllUsers().size() != 1)
		{
			room->removeUser(user);
		}
		else
		{
			handlerFactory->getRoomManager().deleteRoom(room->getRoomData().id);//deleting room
		}

	}
	else
	{
		result.newHandler = Sender;
	}
	result.response = JsonResponsePacketSerializer::serializeResponse(getRoomStateResponse);
	return result;

}



/*
startGame: this function will start the game
input: the request info, the room and factory ptr, and the sender IRequestHandler ptr
output: the request result
*/
RequestResult RoomRequestsHandler::startGame(RequestInfo reqInfo, Room* room, LoggedUser user, RequestHandlerFactory* handlerFactory, IRequestHandler* Sender)
{
	RequestResult result;
	StartGameResponse response;
	response.status = STATUS_POS;
	room->modifyActive(ACTIVE);
	int id = handlerFactory->getGameManager().createGame(*room);
	room->setGameId(id);

	if (room->getAllUsers().size() == 1)
	{
		handlerFactory->getRoomManager().deleteRoom(room->getRoomData().id);//deleting room

	}
	else
	{
		room->AdminLeft();
		room->removeUser(user);
	}


	result.response = JsonResponsePacketSerializer::serializeResponse(response);
	result.newHandler = handlerFactory->createGameRequestHandler(user, handlerFactory->getGameManager().getGame(id));
	return result;
}




/*
handle request: thus function will identify the request type, and then handle it
input: the request info, the room and factory ptr, and the sender IRequestHandler ptr
output: the request result
*/
RequestResult RoomRequestsHandler::closeRoom(RequestInfo reqInfo, Room* room, LoggedUser user, RequestHandlerFactory* handlerFactory, IRequestHandler* Sender)
{
	RequestResult result;
	CloseRoomResponse closeRoomResponse;
	closeRoomResponse.status = STATUS_POS;
	if (room->getAllUsers().size() == 1)
	{
		handlerFactory->getRoomManager().deleteRoom(room->getRoomData().id);//deleting room

	}
	else
	{
		room->AdminLeft();
		room->removeUser(user);
	}
	result.response = JsonResponsePacketSerializer::serializeResponse(closeRoomResponse);
	result.newHandler = handlerFactory->createMenuRequestHandler(user);
	return result;
}


