#pragma once
#include <string>
#include <vector>
#include "IRequestHandler.h"
#include <nlohmann/json.hpp>
#include <algorithm>

using nlohmann::json;
using std::vector;
using std::string;


typedef struct LoginRequest
{
	string userName;
	string password;
};

typedef struct signUpRequest
{
	string userName;
	string password;
	string email;
};

typedef struct GetPlayersRoomRequest
{
	unsigned int roomId;
};

typedef struct JoinRoomRequest {
	unsigned int roomId;
};

typedef struct CreateRoomRequest {
	std::string roomName;
	unsigned int maxUsers;
	unsigned int questionCount;
	unsigned int answerTimeout;
	std::vector<int> difficulties;
	std::vector<std::string> categories;
};

typedef struct SubmitAnswerRequest {
	string answer;
};


class JsonRequestPacketDeserializer
{
public:
	static LoginRequest DeserializeLoginRequest(vector<unsigned char> buffer);
	static signUpRequest DeserializeSignUpRequest(vector<unsigned char> buffer);
	static int DeserializeRequsetLength(vector<unsigned char> lengthInBytes);

	static GetPlayersRoomRequest deserializeGetPlayersRequest(vector<unsigned char> buffer);
	static JoinRoomRequest deserializeJoinRoomRequest(vector<unsigned char> buffer);
	static CreateRoomRequest deserializeCreateRoomRequest(vector<unsigned char> buffer);

	static SubmitAnswerRequest deserializeSubmitAnswerRequest(vector<unsigned char> buffer);
};













