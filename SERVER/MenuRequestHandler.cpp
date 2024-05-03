#include "MenuRequestHandler.h"
/*
isRequestRelevant: this function will check if the code in the request matchs one of the  codes
input: the request info struct
output: a bool saying if the code is valid or not
*/
bool MenuRequestHandler::isRequestRelevant(RequestInfo reqInfo)
{
	if (reqInfo.requestCode == LOGOUT || reqInfo.requestCode == GET_ROOMS || reqInfo.requestCode == GET_PLAYERS_IN_ROOM || reqInfo.requestCode == GET_HIGH_SCORE || reqInfo.requestCode == GET_PERSONAL_STATUS || reqInfo.requestCode == JOIN_ROOM || reqInfo.requestCode == CREATE_ROOM)
	{
		return true;
	}
    return false;
}

/*
C'tor: this function will create a new Menu request handler
input: the logged user, and factory ptr
output: non
*/
MenuRequestHandler::MenuRequestHandler(LoggedUser user, RequestHandlerFactory* factory)
{
	m_user = user;
	m_handlerFactory = factory;
}


/*
D'tor: this function will delete a Menu request handler
input: non
output: non
*/
MenuRequestHandler::~MenuRequestHandler()
{
	m_handlerFactory->getLoginManager().logout(m_user.getUsername());//removing the logged user in case of connection droppes.
}




/*
handleRequest: this function will get the request info, and based on the request, return a request result with a response
input: the request info with the buffer, code, and timestamp of the request
output: the request result: a struct with the response buffer and a pointer to the next handler
*/
RequestResult MenuRequestHandler::handleRequest(RequestInfo reqInfo)
{
	RequestResult result;
	ErrorResponse err;
	RequestResult errCase;
	try
	{
		if (isRequestRelevant(reqInfo))
		{
			if (reqInfo.requestCode == LOGOUT)
			{
				return logout(reqInfo);
			}
			else if (reqInfo.requestCode == GET_ROOMS)
			{
				return getRooms(reqInfo);
			}
			else if (reqInfo.requestCode == GET_PLAYERS_IN_ROOM)
			{
				return getPlayersInRoom(reqInfo);
			}
			else if (reqInfo.requestCode == GET_HIGH_SCORE)
			{
				return getHighScore(reqInfo);
			}
			else if (reqInfo.requestCode == GET_PERSONAL_STATUS)
			{
				return getPersonalStats(reqInfo);
			}	
			else if (reqInfo.requestCode == JOIN_ROOM)
			{
				return joinRoom(reqInfo);
			}
			else if (reqInfo.requestCode == CREATE_ROOM)
			{
				return createRoom(reqInfo);
			}

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


/*
logout: this function will log out a user
input: the request info
output: the requestResult.
*/
RequestResult MenuRequestHandler::logout(RequestInfo reqInfo)
{
	RequestResult result;
	LogoutResponse logoutResponse;
	if (m_handlerFactory->getLoginManager().logout(m_user.getUsername()))// trying to log out. logout only uses the code.
	{
		result.newHandler = m_handlerFactory->createLoginRequestHandler();
		logoutResponse.status = STATUS_POS;
		result.response = JsonResponsePacketSerializer::serializeResponse(logoutResponse);
	}
	else// if this happens something isn't working right
	{
		result.newHandler = this;
		logoutResponse.status = STATUS_NEG;
		result.response = JsonResponsePacketSerializer::serializeResponse(logoutResponse);
	}
	return result;
}



/*
this function will get all of the rooms into a response
input: the request info
output: the requestResult.
*/
RequestResult MenuRequestHandler::getRooms(RequestInfo reqInfo)
{
	RequestResult result;
	GetRoomResponse getRoomsResponse;
	getRoomsResponse.rooms = m_handlerFactory->getRoomManager().getRooms();// getting rooms
	if (getRoomsResponse.rooms.empty())
	{
		getRoomsResponse.status = STATUS_NEG;
	}
	else
	{
		getRoomsResponse.status = STATUS_POS;
	}
	result.response = JsonResponsePacketSerializer::serializeResponse(getRoomsResponse);
	result.newHandler = this;// returning itself
	return result;
}



/*
getHighScores: this function will give a requestResult with the highest scores
input: the request info
output: the requestResult.
*/
RequestResult MenuRequestHandler::getHighScore(RequestInfo reqInfo)
{
	RequestResult result;
	getHighScoreResponse highScores;
	highScores.statistics = m_handlerFactory->getStatisticManager().getHighScore();// getting scores
	if (highScores.statistics.empty())
	{
		highScores.status = STATUS_NEG;
	}
	else
	{
		highScores.status = STATUS_POS;
	}
	result.response = JsonResponsePacketSerializer::serializeResponse(highScores);
	result.newHandler = this;// returning itself
	return result;

}

/*
getPlayersInRoom: this function will give a requestResult with all of the players in a specific room
input: the request info
output: the requestResult.
*/
RequestResult MenuRequestHandler::getPlayersInRoom(RequestInfo reqInfo)
{
	RequestResult result;
	GetPlayersRoomRequest request;
	GetPlayersInRoomResponse response;
	Room* room = nullptr;
	request.roomId = JsonRequestPacketDeserializer::deserializeGetPlayersRequest(reqInfo.buffer).roomId;// getting the id of the room
	room = m_handlerFactory->getRoomManager().getRoom(request.roomId);//trying to get room
	if (room != nullptr)
	{
		response.players = room->getAllUsers();
	}
	result.response = JsonResponsePacketSerializer::serializeResponse(response);//seriallizing
	result.newHandler = this;
	return result;
}




/*
getPersonalStats: this function will give a requestResult with all of stats of the user
input: the request info
output: the requestResult.
*/
RequestResult MenuRequestHandler::getPersonalStats(RequestInfo reqInfo)
{
	RequestResult result;
	getPersonalStatusResponse response;
	response.statistics = m_handlerFactory->getStatisticManager().getUserStatistics(m_user.getUsername());//getting stats
	response.status = STATUS_POS;
	result.response = JsonResponsePacketSerializer::serializeResponse(response);
	result.newHandler = this;
	return result;
}









/*
joinRoom: this function will join the user into a room
input: the request info
output: the requestResult.
*/
RequestResult MenuRequestHandler::joinRoom(RequestInfo reqInfo)
{
	RequestResult result;
	JoinRoomRequest request;
	JoinRoomResponse response;
	Room* room = nullptr;
	request.roomId = JsonRequestPacketDeserializer::deserializeJoinRoomRequest(reqInfo.buffer).roomId;//getting room id
	room = m_handlerFactory->getRoomManager().getRoom(request.roomId);//getting room
	if (room != nullptr && m_handlerFactory->getRoomManager().getRoomState(request.roomId) == INACTIVE && room->getAllUsers().size() < room->getRoomData().maxPlayers)// checking if the room exists, and inactive
	{
		room->addUser(m_user);
		response.status = STATUS_POS;
		result.newHandler = m_handlerFactory->createRoomMemberRequestHandler(m_user, room);//going to next handler as member

	}
	else
	{
		response.status = STATUS_NEG;
		result.newHandler = this;

	}
	result.response = JsonResponsePacketSerializer::serializeResponse(response);
	return result;
}








/*
createRoom: this function will create a new room for the user
input: the request info
output: the requestResult.
*/
RequestResult MenuRequestHandler::createRoom(RequestInfo reqInfo)
{
	RequestResult result;
	CreateRoomRequest request;
	CreateRoomResponse response;
	RoomData data;
	bool isDiffValid = true, isCategoriesValid = true;
	//getting values into the request
	data = getCreateRoomRequestRoomData(JsonRequestPacketDeserializer::deserializeCreateRoomRequest(reqInfo.buffer));
	data.id = m_handlerFactory->getRoomManager().generateNewId();

	for (auto itt2 : data.difficulties)//these 2 loops will make sure that the difficulties and categories are valid
	{
		if (itt2 != EASY && itt2 != MEDIUM && itt2 != HARD)
		{
			isDiffValid = false;
		}
	}
	for (auto itt3 : data.categories)
	{
		if (itt3 != "Films" && itt3 != "Video Games" && itt3 != "Television" && itt3 != "General Knowledge" && itt3 != "Mythology" && itt3 != "Sports" && itt3 != "History")
		{
			isCategoriesValid = false;
		}
	}
	if (isDiffValid == true && isCategoriesValid == true)
	{
		m_handlerFactory->getRoomManager().createRoom(m_user, data);
		response.status = STATUS_POS;
		result.newHandler = m_handlerFactory->createRoomAdminRequestHandler(m_user, m_handlerFactory->getRoomManager().getRoom(data.id));
	}
	else
	{
		response.status = STATUS_NEG;
		result.newHandler = this;
	}
	result.response = JsonResponsePacketSerializer::serializeResponse(response);
	return result;




}




/*
getCreateRoomRequestRoomData: this function will extract the Create room request into a RoomData
input: the request
output: the data
*/
RoomData MenuRequestHandler::getCreateRoomRequestRoomData(CreateRoomRequest req)
{
	RoomData data;
	//putting values in the roomData
	data.categories = req.categories;//setting all fields
	data.difficulties = req.difficulties;
	data.isActive = INACTIVE;
	data.maxPlayers = req.maxUsers;
	data.name = req.roomName;
	data.numOfQuestionsInGame = req.questionCount;
	data.timePerQuestion = req.answerTimeout;
	data.currNumOfPlayers = 1;// for these fields, setting default values
	data.id = 0;
	data.isAdminStillInRoom = true;
	data.gameId = 0;
	return data;
}


















