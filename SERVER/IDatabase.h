#pragma once
#include <string>
#include "sqlite3.h"
#include <io.h>
#include "User.h"
#include <vector>
#include <algorithm>
#include <list>
#include "Game.h"
#define EASY 1
#define MEDIUM 2
#define HARD 3


typedef struct Question
{
	std::string category;
	std::string question;
	std::string falseAns1;
	std::string falseAns2;
	std::string falseAns3;
	std::string correctAns;
	int difficult;

};

typedef struct Statistic
{
	std::string userName;
	int correctAns;
	int totalGames;
	int totalAns;
	float avgTime;
	int score;
};

struct GameData;

class IDatabase
{
public:
	virtual bool open() = 0;
	virtual bool close() = 0;
	virtual int doesUserExist(std::string name) = 0;
	virtual int doesPasswordMatch(std::string name, std::string password) = 0;
	virtual int addNewUser(std::string name, std::string password, std::string mail) = 0;

	virtual std::vector<Question> getQuestions(std::vector<int> difficulties, std::vector<std::string>categories, int numberOfQuestions) = 0;
	virtual float getPlayerAvarageAnswerTime(std::string name) = 0;
	virtual int getNumOfCorrectAnswers(std::string name) = 0;
	virtual int getNumOfTotalAnswers(std::string name) = 0;
	virtual int getNumOfPlayerGames(std::string name) = 0;
	virtual int getPlayerScore(std::string name) = 0;
	virtual std::list<Statistic> getHighScores() = 0;

	virtual int submitGameStatistics(GameData myData, std::string username) = 0;
};

