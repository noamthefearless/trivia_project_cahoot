#pragma once
#include "LoggedUser.h"
#include <vector>
#define ACTIVE 1
#define INACTIVE 0


typedef struct RoomData {
	int id;
	std::string name;
	unsigned int maxPlayers;
	unsigned int currNumOfPlayers;
	unsigned int numOfQuestionsInGame;
	unsigned int timePerQuestion;
	unsigned int isActive;
	std::vector<std::string> categories;
	std::vector<int> difficulties;
	bool isAdminStillInRoom;
	unsigned int gameId;
};


class Room
{
public:
	Room() = default;
	Room(LoggedUser logUs, RoomData myData);
	~Room() = default;
	void addUser(LoggedUser);
	void removeUser(LoggedUser);
	std::vector<std::string> getAllUsers();
	RoomData getRoomData();
	bool isAdminStillInRoom();
	void AdminLeft();
	void modifyActive(unsigned int activeOrInactive);
	void setGameId(unsigned int gameId);

private:
	RoomData m_metadata;
	std::vector<LoggedUser> m_users;
};

