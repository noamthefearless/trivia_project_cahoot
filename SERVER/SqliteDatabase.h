#pragma once
#include "IDatabase.h"
#include <list>
#define DB_FILE_NAME "TRIVIADB.sqlite"
class SqliteDatabase : public IDatabase
{
public:
	SqliteDatabase();
	~SqliteDatabase();
	virtual bool open();
	virtual bool close();
	virtual int doesUserExist(std::string name);
	virtual int doesPasswordMatch(std::string name, std::string password);
	virtual int addNewUser(std::string name, std::string password, std::string mail);

	virtual std::vector<Question> getQuestions(std::vector<int> difficulties, std::vector<std::string>categories, int numberOfQuestions);
	virtual float getPlayerAvarageAnswerTime(std::string name);
	virtual int getNumOfCorrectAnswers(std::string name);
	virtual int getNumOfTotalAnswers(std::string name);
	virtual int getNumOfPlayerGames(std::string name);
	virtual int getPlayerScore(std::string name);
	virtual std::list<Statistic> getHighScores();

	virtual int submitGameStatistics(GameData myData, std::string username);



	//non virtual functions:
	std::string getCorrectAnswer(std::string question);
	int getNumberOfQuestions() const;
	std::list<Statistic> getAllStats(std::string name) const;
	std::list<User> getAllUserData(std::string name) const;
private:
	sqlite3* m_db;



};

