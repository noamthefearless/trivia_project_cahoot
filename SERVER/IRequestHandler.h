#pragma once
#include<ctime>
#include<vector>
#include <nlohmann/json.hpp>
#include "JsonRequestPacketDeserializer.h"
#include "JsonResponsePacketSerializer.h"
#include "RoomManager.h"
#define ERROR_CODE 200
#define STATUS_POS 1
#define STATUS_NEG 0
using std::vector;
using std::time_t;



enum requestCodes
{
	LOGIN = 1, SIGN_UP = 2, LOGOUT = 3, GET_ROOMS = 4, GET_PLAYERS_IN_ROOM = 5, GET_HIGH_SCORE = 6,
	GET_PERSONAL_STATUS = 7, JOIN_ROOM = 8, CREATE_ROOM = 9, CLOSE_ROOM = 10, START_GAME = 11, GET_ROOM_STATE = 12, LEAVE_ROOM = 13, LEAVE_GAME = 14, GET_QUESTION = 15, SUBMIT_ANSWER = 16, 
	GET_GAME_RESULTS = 17
};

enum responseCodes
{
	LOGIN_RESPONSE = 101, SIGN_UP_RESPONSE = 102, LOGOUT_RESPONSE = 103, GET_ROOMS_RESPONSE = 104,
	GET_PLAYERS_IN_ROOM_RESPONSE = 105, GET_HIGH_SCORE_RESPONSE = 106, GET_PERSONAL_STATUS_RESPONSE = 107, JOIN_ROOM_RESPONSE = 108,
	CREATE_ROOM_RESPONSE = 109, CLOSE_ROOM_RESPONSE = 110, START_GAME_RESPONSE = 111, GET_ROOM_STATE_RESPONSE = 112, LEAVE_ROOM_RESPONSE = 113, LEAVE_GAME_RESPONSE = 114, 
	GET_QUESTION_RESPONSE = 115, SUBMIT_ANSWER_RESPONSE = 116, GET_GAME_RESULTS_RESPONSE = 117
};

class IRequestHandler;


typedef struct RequestInfo
{
	int requestCode;
	time_t receivalTime;
	vector<unsigned char> buffer;
};

typedef struct RequestResult
{
	vector<unsigned char> response;
	IRequestHandler* newHandler;
};


class IRequestHandler
{
public:
	virtual bool isRequestRelevant(RequestInfo reqInfo) = 0;
	virtual RequestResult handleRequest(RequestInfo reqInfo) = 0;
};




