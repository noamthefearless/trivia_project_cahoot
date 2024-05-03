#pragma once

#include <fstream>
#include <nlohmann/json.hpp>
#include "IRequestHandler.h"
#include "Room.h"
#include "IDatabase.h"



using json = nlohmann::json;

typedef struct {
	unsigned int status;
}LoginResponse;

typedef struct {
	unsigned int status;
}SignupResponse;

typedef struct {
	std::string message;
}ErrorResponse;

typedef struct {
	unsigned int status;
}LogoutResponse;

typedef struct {
	unsigned int status;
	std::vector<RoomData> rooms;
}GetRoomResponse;

typedef struct {
	std::vector<std::string> players;
}GetPlayersInRoomResponse;

typedef struct {
	unsigned int status;
	std::list<Statistic> statistics;
}getHighScoreResponse;

typedef struct{
	unsigned int status;
	Statistic statistics;
}getPersonalStatusResponse;

typedef struct {
	unsigned int status;
}JoinRoomResponse;

typedef struct {
	unsigned int status;
}CloseRoomResponse;

typedef struct {
	unsigned int status;
}CreateRoomResponse;

typedef struct {
	unsigned int status;
}StartGameResponse;

typedef struct {
	unsigned int status;
	bool hasGameBegun;
	std::vector<std::string> players;
	unsigned int questionCount;
	unsigned int answerTimeout;
}GetRoomStateResponse;

typedef struct {
	unsigned int status;
}LeaveRoomResponse;

typedef struct {
	unsigned int status;
}LeaveGameResponse;

typedef struct {
	unsigned int status;
	Question question;
	unsigned int questionIndex;
	unsigned int numOfQuestions;
	unsigned int timeForQuestion;
}GetQuestionResponse;

typedef struct {
	unsigned int status;
	std::string correctAnswer;
	unsigned int points;
	unsigned int streak;
}SubmitAnswerResponse;

typedef struct {
	std::string username;
	unsigned int correctAnswerCount;
	unsigned int wrongAnswerCount;
	unsigned int averageAnswerTime;
	unsigned int points;
}PlayerResults;

typedef struct {
	unsigned int status;
	std::vector<PlayerResults> results;
}GetGameResultsResponse;


class JsonResponsePacketSerializer
{
public:

	static std::vector<unsigned char> serializeResponse(ErrorResponse myErr);
	static std::vector<unsigned char> serializeResponse(SignupResponse mySignup);
	static std::vector<unsigned char> serializeResponse(LoginResponse myLogin);

	static std::vector<unsigned char> serializeResponse(LogoutResponse myLogout);
	static std::vector<unsigned char> serializeResponse(GetRoomResponse myRoom);
	static std::vector<unsigned char> serializeResponse(JoinRoomResponse myRoom);
	static std::vector<unsigned char> serializeResponse(CreateRoomResponse myRoom);
	static std::vector<unsigned char> serializeResponse(getHighScoreResponse myScore);
	static std::vector<unsigned char> serializeResponse(getPersonalStatusResponse myStatus);
	static std::vector<unsigned char> serializeResponse(GetPlayersInRoomResponse myRoom);

	static std::vector<unsigned char> serializeResponse(CloseRoomResponse myRoom);
	static std::vector<unsigned char> serializeResponse(StartGameResponse myRoom);
	static std::vector<unsigned char> serializeResponse(GetRoomStateResponse myRoom);
	static std::vector<unsigned char> serializeResponse(LeaveRoomResponse myRoom);

	static std::vector<unsigned char> serializeResponse(GetGameResultsResponse myResponse);
	static std::vector<unsigned char> serializeResponse(SubmitAnswerResponse myResponse);
	static std::vector<unsigned char> serializeResponse(GetQuestionResponse myResponse);
	static std::vector<unsigned char> serializeResponse(LeaveGameResponse myResponse);
};

