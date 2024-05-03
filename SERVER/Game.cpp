#include "Game.h"





/*
C'tor: this function will create a new game
input: the id and answer timeout, along with the users, and questions in a vector
output: non
*/
Game::Game(unsigned int id, unsigned int answerTimeout, std::vector<LoggedUser> players, std::vector<Question> questions)
{
	m_gameId = id;
	m_answerTimeout = answerTimeout;
	m_questions = questions;
	for (int i = 0; i < players.size(); i++)
	{
		m_players[players[i]] = GameData();//putting all players in the map
	}
}



/*
getQuestionForUser: this function will get the next question for the user
input: the user
output: the question that the user needs next
*/
Question Game::getQuestionForUser(LoggedUser user)
{
	return m_questions[getPlayerData(user).currentQuestionIndex];
}



/*
getPlayerData: this function will get a gameData struct for a specific user
input: the logged user
output: the game data
*/
GameData Game::getPlayerData(LoggedUser user)
{
	return m_players[user];
}


/*
removePlayer: this function will remove a player from a room
input: non
output: non
*/
void Game::removePlayer(LoggedUser user)
{
	m_players.erase(user);
}




/*
getPlayers: this function will get all of the players in the game, and return them in a vector
input: non
output: a vector of all of the players
*/
std::vector<LoggedUser> Game::getPlayers()
{
	std::vector<LoggedUser> result;

	for (auto itt : m_players)//itterating
	{
		result.push_back(itt.first);
	}
	return result;
}



/*
getAnswerTimeOut: this function will return the number of seconds for each question in the game
input: non
output: the answer timeout
*/
unsigned int Game::getAnswerTimeout()
{
	return m_answerTimeout;
}


/*
getAnswerTimeOut: this function will return the id of the game
input: non
output: the id
*/
unsigned int Game::getId()
{

	return m_gameId;
	
}




/*
updateData: this function will update the game data of a player in the game 
input: the new data to update to, and the user who is getting updated
output: non
*/
void Game::updateData(LoggedUser user, GameData newGameData)
{
	m_players[user].averageAnswerTime =  newGameData.averageAnswerTime;
	m_players[user].correctAnswerCount = newGameData.correctAnswerCount;
	m_players[user].currentQuestionIndex = newGameData.currentQuestionIndex;
	m_players[user].points = newGameData.points;
	m_players[user].streak = newGameData.streak;
	m_players[user].wrongAnswerCount = newGameData.wrongAnswerCount;
}


/*
getNumOfQuestions: this function will get the number of question in the game
input: non
output: the number of questions
*/
int Game::getNumOfQuestions()
{
	return m_questions.size();
}





/*
isGameFinished: this function will check if all of the players have finished the game 
input: non
output: a bool 
*/
bool Game::isGameFinished()
{
	for (auto itt : m_players)
	{
		if (itt.second.currentQuestionIndex != getNumOfQuestions())// checking if there is a player still playing
		{
			return false;
		}
	}
	return true;
}





/*
getGameResults: this function will get the game results once the game is over 
input: non
output: a vector of gameData. it will be empty if the game is not finished
*/
std::map<std::string, GameData> Game::getGameResults()
{
	std::map<std::string, GameData> result;
	std::vector<LoggedUser> users = getPlayers();
	if (isGameFinished())
	{
		for (int i = 0; i < users.size(); i++)//this is the only way to access the game data and the user name as well
		{
			result[users[i].getUsername()] = m_players[users[i]];
		}
	}
	return result;
}







