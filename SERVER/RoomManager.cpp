#include "RoomManager.h"


/// <summary>
/// This fubnction created a new room
/// </summary>
/// <param name="logUs">The admin of the room</param>
/// <param name="roomData">The data of the room</param>
void RoomManager::createRoom(LoggedUser logUs, RoomData roomData)
{
	m_rooms[roomData.id] = Room(logUs, roomData);
}

/// <summary>
/// This function deleted a room
/// </summary>
/// <param name="ID">The room's ID to be deleted</param>
void RoomManager::deleteRoom(int ID)
{
	m_rooms.erase(ID);
}

/// <summary>
/// This function checks if the rooms is active or not
/// </summary>
/// <param name="ID">The id of the room to be checked</param>
/// <returns>If the room is active or not</returns>
unsigned int RoomManager::getRoomState(int ID)
{
	Room* myRoom;
	for (auto& pair : m_rooms)
	{
		if (pair.first == ID)
		{
			myRoom = &pair.second;
			return myRoom->getRoomData().isActive;
		}
	}
	return 0;
}

/// <summary>
/// This function get all the room
/// </summary>
/// <returns>All the rooms</returns>
std::vector<RoomData> RoomManager::getRooms()
{
	std::vector<RoomData> myRoomData;
	Room* myRoom;
	for (auto& pair : m_rooms)
	{
		myRoom = &pair.second;
		if (myRoom->isAdminStillInRoom())
		{
			myRoomData.push_back(myRoom->getRoomData());

		}
	}
	return myRoomData;
}

/// <summary>
/// This function get a specific room
/// </summary>
/// <param name="ID">The room's ID</param>
/// <returns>The room</returns>
Room* RoomManager::getRoom(int ID)
{
	for (auto& pair : m_rooms)
	{
		if (pair.first == ID)
		{
			return &pair.second;
		}
	}
	return nullptr;
}




/*
generateNewId: this function will generate a new room id
input: non 
output: an id that is not in any room
*/
int RoomManager::generateNewId() const
{
	int id = 1;
	while (m_rooms.find(id) != m_rooms.end())//finding an id that is not in any room
	{
		id++;
	}
	return id;
}


