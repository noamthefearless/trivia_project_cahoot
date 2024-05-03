#pragma once
#include <vector>
#include <string>
#include "IDatabase.h"
class StatisticsManager
{
public:
	std::vector<std::string> getHighScore();
	std::vector<std::string> getUserStatistics(std::string userName);

private:
	IDatabase* m_database;
};

