#include"GameManager.h"






GameManager::GameManager(IDatabase* db)
{
	m_database = db;
}





/*
createGmae: this function will create a new game into the game map
input: the room
output: non
*/
int GameManager::createGame(Room myRoom)
{
	unsigned int id = 1;
	RoomData Rd = myRoom.getRoomData();
	std::vector<Question> questions = m_database->getQuestions(Rd.difficulties, Rd.categories, Rd.numOfQuestionsInGame);
	std::vector<LoggedUser> users;
	std::vector<std::string> userNames = myRoom.getAllUsers();
	while (m_games.find(id) != m_games.end())
	{
		id++;
	}

	for (int i = 0; i < userNames.size(); i++)
	{
		users.push_back(LoggedUser(userNames[i]));//converting into a logged user vector
	}
	m_games[id] = Game(id, Rd.timePerQuestion, users, questions);
	return id;
}


/*
deleteGame: this function will delete a game from the list
input: the game id
output: non
*/
void GameManager::deleteGame(unsigned int gameId)
{
	m_games.erase(gameId);
}


/*
getGame: this function will return a pointer to a game
input: the game id
output: the game ptr or a nullptr
*/
Game* GameManager::getGame(unsigned int Id)
{
	for (auto& pair : m_games)
	{
		if (pair.first == Id)
		{
			return &pair.second;
		}
	}
	return nullptr;
}


/*
submitScore: this function will submit the game stats to db
input: the user, and room id
output: non
*/
void GameManager::submitScore(unsigned int Id, LoggedUser user)
{
	m_database->submitGameStatistics(m_games[Id].getPlayerData(user), user.getUsername());
}







