#include "StatisticManager.h"

/// <summary>
/// This function gets the 5th highest player's scores
/// </summary>
/// <returns>The 5th highest player's scores</returns>
std::list<Statistic> StatisticManager::getHighScore()
{
    return m_database->getHighScores();
}

/*
C'tor: this function will create a new statisticsManager
input: an IDatabase ptr
output: non
*/
StatisticManager::StatisticManager(IDatabase* database)
{
    m_database = database;
}




/// <summary>
/// This function gets all the statistics of the user in statistic format
/// </summary>
/// <param name="username">The name of the player</param>
/// <returns>The statistics of the player</returns>
Statistic StatisticManager::getUserStatistics(std::string username)
{
    Statistic stats;
    stats.avgTime = m_database->getPlayerAvarageAnswerTime(username);
    stats.correctAns = m_database->getNumOfCorrectAnswers(username);
    stats.score = m_database->getPlayerScore(username);
    stats.totalAns = m_database->getNumOfTotalAnswers(username);
    stats.totalGames = m_database->getNumOfPlayerGames(username);
    stats.userName = username;
    return stats;
}
