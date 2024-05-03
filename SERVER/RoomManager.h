#pragma once
#include "Room.h"
#include <map>
#include <utility>
#include<cstdlib>

class RoomManager
{
public:
	void createRoom(LoggedUser logUs, RoomData roomData);
	void deleteRoom(int ID);
	unsigned int getRoomState(int ID);
	std::vector<RoomData> getRooms();
	Room* getRoom(int ID);
	int generateNewId() const;

private:
	std::map<int, Room> m_rooms;
};

