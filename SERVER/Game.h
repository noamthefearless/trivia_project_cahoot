#pragma once
#include <vector>
#include <map>
#include "LoggedUser.h"
#include "IDatabase.h"


typedef struct GameData {
	unsigned int currentQuestionIndex = 0;
	unsigned int correctAnswerCount = 0;
	unsigned int wrongAnswerCount = 0;
	unsigned int averageAnswerTime = 0;
	unsigned int points = 0;
	unsigned int streak = 0;
};

struct Question;


class Game
{
public:
	Game() = default;
	Game(unsigned int id, unsigned int answerTimeout, std::vector<LoggedUser> players, std::vector<Question> questions);
	Question getQuestionForUser(LoggedUser user);
	void removePlayer(LoggedUser user);
	std::vector<LoggedUser> getPlayers();
	GameData getPlayerData(LoggedUser user);
	void updateData(LoggedUser user, GameData newGameData);
	unsigned int getId();
	unsigned int getAnswerTimeout();
	int getNumOfQuestions();
	std::map<std::string, GameData> getGameResults();


private:
	unsigned int m_gameId;
	unsigned int m_answerTimeout;
	std::map<LoggedUser, GameData> m_players;
	std::vector<Question> m_questions;
	bool isGameFinished();


};

