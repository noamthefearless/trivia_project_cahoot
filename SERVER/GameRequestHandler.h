#pragma once
#include "IRequestHandler.h"
#include "RequestHandlerFactory.h"
#include "IDatabase.h"
#include "Game.h"
#include <algorithm>

class GameRequestHandler : public IRequestHandler
{
private:
	Game* m_game;
	LoggedUser m_user;
	RequestHandlerFactory* m_handlerFactory;
	time_t m_lastSentQuestion;
	
	RequestResult getQuestion(RequestInfo info);
	RequestResult submitAnswer(RequestInfo info);
	RequestResult getGameResults(RequestInfo info);
	RequestResult leaveGame(RequestInfo info);
	int calcPoints(int time);

public:
	virtual bool isRequestRelevant(RequestInfo info);
	virtual RequestResult handleRequest(RequestInfo myInfo);

	GameRequestHandler(LoggedUser myUser, RequestHandlerFactory* myFactory, Game* myGame);
	GameRequestHandler() = default;
	~GameRequestHandler();
};

