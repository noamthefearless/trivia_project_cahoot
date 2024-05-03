#include "Room.h"

Room::Room(LoggedUser logUs, RoomData myData)
{
	m_metadata = myData;
	m_users.push_back(logUs);
}

/// <summary>
/// This function add a user to the logged users in the room
/// </summary>
/// <param name="logUs">The user to be added</param>
void Room::addUser(LoggedUser logUs)
{
	m_users.push_back(logUs);
	m_metadata.currNumOfPlayers++;
}


/// <summary>
/// This function remove a user from the logged users in the room
/// </summary>
/// <param name="logUs">The user to be removed</param>
void Room::removeUser(LoggedUser logUs)
{
	for (auto it = m_users.begin(); it != m_users.end(); it++)
	{
		if ((*it).getUsername() == logUs.getUsername())
		{
			m_users.erase(it);
			m_metadata.currNumOfPlayers--;
			return;
		}
	}
}


/// <summary>
/// This function gets all the logged users in the room
/// </summary>
/// <returns>The logged users in the room</returns>
std::vector<std::string> Room::getAllUsers()
{
	std::vector<std::string> newVec;
	for (auto it = m_users.begin(); it != m_users.end(); it++)
	{
		newVec.push_back((*it).getUsername());
	}
	return newVec;
}

/// <summary>
/// This function gets the room data
/// </summary>
/// <returns>The room data</returns>
RoomData Room::getRoomData()
{
	return m_metadata;
}

void Room::modifyActive(unsigned int activeOrInactive)
{
	this->m_metadata.isActive = activeOrInactive;
}



/*
isAdminStillInRoom: this function will say if the admin is still in the room
input: non
output: boolean
*/
bool Room::isAdminStillInRoom()
{
	return m_metadata.isAdminStillInRoom;
}


/*
this function will indicate that the admin left
input: non
output: non
*/
void Room::AdminLeft()
{
	m_metadata.isAdminStillInRoom = false;
}



/*
this function will set the game id
input: the id
output: non
*/
void Room::setGameId(unsigned int gameId)
{
	m_metadata.gameId = gameId;
}






