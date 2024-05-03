#include "RequestHandlerFactory.h"

/*
C'tor: this function will create a new Request Handler Factory
input: the database and tne Managers
output: non
*/
RequestHandlerFactory::RequestHandlerFactory(LoginManager loginManager, RoomManager roomManager, StatisticManager statisticsManager, GameManager gameManager,IDatabase* myDB) : m_database(myDB), m_loginManager (loginManager), m_roomManager (roomManager), m_statisticsManager (statisticsManager), m_gameManager (gameManager)
{
    //empty
}

/*
D'tor: this function will delete a Request Handler Factory
input: non
output: non
*/
RequestHandlerFactory::~RequestHandlerFactory()
{
    
}

/*
createLoginRequestHandler: this function will create and return a ptr to a new LoginRequestHandler
input: non
output: the loginRequestHandler
*/
LoginRequestHandler* RequestHandlerFactory::createLoginRequestHandler() 
{
    return new LoginRequestHandler(this);
}

/*
createMenuRequestHandler: this function will create a new MenuRequest Handler
input: the logged user
output: the ptr for the handler
*/
MenuRequestHandler* RequestHandlerFactory::createMenuRequestHandler(LoggedUser user)
{
    return new MenuRequestHandler(user, this);
}

/*
getLoginManager: this function will getTheLoginManager
input: non
output: the loginManager
*/
LoginManager& RequestHandlerFactory::getLoginManager() 
{
    return m_loginManager;
}

/*
getRoomManager: this function will get The RoomManager
input: non
output: the RoomManager
*/
RoomManager& RequestHandlerFactory::getRoomManager()
{
    return m_roomManager;
}

/*
getStatisticManager: this function will get The StatisticManager
input: non
output: the StatisticManager
*/
StatisticManager& RequestHandlerFactory::getStatisticManager()
{
    return m_statisticsManager;
}


/*
createGameRequestHandler: this function will create a new GameRequest Handler
input: the logged user and a pter to the game
output: the ptr for the handler
*/
GameRequestHandler* RequestHandlerFactory::createGameRequestHandler(LoggedUser myUser, Game* myGame)
{
    return new GameRequestHandler(myUser, this, myGame);
}

/*
getGameManager: this function will get The GameManager
input: non
output: the GameManager
*/
GameManager& RequestHandlerFactory::getGameManager()
{
    return m_gameManager;
}

/// <summary>
/// This function creates a new room admin request handler
/// </summary>
/// <param name="myUser">The logged user</param>
/// <param name="myRoom">The admin's room</param>
/// <returns>The new handler</returns>
RoomAdminRequestHandler* RequestHandlerFactory::createRoomAdminRequestHandler(LoggedUser myUser, Room* myRoom)
{
    return new RoomAdminRequestHandler(myRoom, myUser, this);
}



/// <summary>
/// this function creates a new room member request handler
/// </summary>
/// <param name="myUser">The logged user</param>
/// <param name="myRoom">The room</param>
/// <returns>The new handler</returns>
RoomMemberRequestHandler* RequestHandlerFactory::createRoomMemberRequestHandler(LoggedUser myUser, Room* myRoom)
{
    return new RoomMemberRequestHandler(myRoom, myUser, this);
}













