#pragma once
#include "IRequestHandler.h"

#include "LoginManager.h"
#include "LoginRequestHandler.h"
#include "MenuRequestHandler.h"
#include "RoomAdminRequestHandler.h"
#include "RoomMemberRequestHandler.h"
#include "RoomManager.h"
#include "StatisticManager.h"
#include "GameManager.h"
#include "GameRequestHandler.h"

class LoginRequestHandler;
class MenuRequestHandler;
class RoomAdminRequestHandler;
class RoomMemberRequestHandler;
class GameRequestHandler;

class RequestHandlerFactory
{
public:
	RequestHandlerFactory() = default;
	RequestHandlerFactory(LoginManager loginManager, RoomManager roomManager,  StatisticManager statisticsManager, GameManager gameManager, IDatabase* myDB);
	~RequestHandlerFactory();
	LoginRequestHandler* createLoginRequestHandler();
	MenuRequestHandler* createMenuRequestHandler(LoggedUser user);
	RoomAdminRequestHandler* createRoomAdminRequestHandler(LoggedUser myUser, Room* myRoom);
	RoomMemberRequestHandler* createRoomMemberRequestHandler(LoggedUser myUser, Room* myRoom);
	LoginManager& getLoginManager();
	RoomManager& getRoomManager();
	StatisticManager& getStatisticManager();
	GameRequestHandler* createGameRequestHandler(LoggedUser myUser, Game* myGame);
	GameManager& getGameManager();

private:
	LoginManager m_loginManager;
	RoomManager m_roomManager;
	StatisticManager m_statisticsManager;
	IDatabase* m_database;
	GameManager m_gameManager;
};

