#pragma once
#include "IDatabase.h"

class StatisticManager
{
public:
	StatisticManager() = default;
	StatisticManager(IDatabase* database);
	std::list<Statistic> getHighScore();
	Statistic getUserStatistics(std::string username);

private:
	IDatabase* m_database;
};

