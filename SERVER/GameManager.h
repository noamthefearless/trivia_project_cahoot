#pragma once
#include "Game.h"
#include "SqliteDatabase.h"
#include "Room.h"
#include <map>
class GameManager
{
public:
	GameManager() = default;
	GameManager(IDatabase* db);
	int createGame(Room myRoom);
	void deleteGame(unsigned int gameId);
	Game* getGame(unsigned int Id);
	void submitScore(unsigned int Id, LoggedUser user);


private:
	IDatabase* m_database;
	std::map<unsigned int, Game> m_games;
};

