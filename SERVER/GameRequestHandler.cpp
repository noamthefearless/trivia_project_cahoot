#include "GameRequestHandler.h"


/*
C'tor: this function will create a new GameRequestHandler
input: the user, and the ptrs for the factory and the game
output: non
*/
GameRequestHandler::GameRequestHandler(LoggedUser myUser, RequestHandlerFactory* myFactory, Game* myGame)
{
    m_handlerFactory = myFactory;
    m_user = myUser;
    m_game = myGame;
}


/*
D'tor: this function will delete a GameRequestHandler
input: non
output: non
*/
GameRequestHandler::~GameRequestHandler()
{
    //Empty
}


/*
isRequsetRelevant: this function will check if the request is relevant to the handler
input: the request info
output: a bool saying if its relevant or not
*/
bool GameRequestHandler::isRequestRelevant(RequestInfo info)
{
    if (info.requestCode == LEAVE_GAME || info.requestCode == GET_QUESTION || info.requestCode == SUBMIT_ANSWER || info.requestCode == GET_GAME_RESULTS)
    {
        return true;
    }
    return false;
}



/*
handleRequest: this function will get the request info, and based on the request, return a request result with a response
input: the request info with the buffer, code, and timestamp of the request
output: the request result: a struct with the response buffer and a pointer to the next handler
*/
RequestResult GameRequestHandler::handleRequest(RequestInfo myInfo)
{
    RequestResult result;
    ErrorResponse err;
    RequestResult errCase;
    try {
        if (isRequestRelevant(myInfo))
        {
            if (myInfo.requestCode == LEAVE_GAME)
            {
                return leaveGame(myInfo);
            }
            if (myInfo.requestCode == GET_QUESTION)
            {
                return getQuestion(myInfo);
            }
            if (myInfo.requestCode == SUBMIT_ANSWER)
            {
                return submitAnswer(myInfo);
            }
            if (myInfo.requestCode == GET_GAME_RESULTS)
            {
                return getGameResults(myInfo);
            }
        }
        else {
            throw std::runtime_error("invalid code");
        }
    }
    catch (...)
    {
        err.message = "Message is invalid! please make sure that the code, length, and data are all valid before sending";
        errCase.response = JsonResponsePacketSerializer::serializeResponse(err);
        errCase.newHandler = this;
        return errCase;
    }
    return result;
}


/*
getQuestion: this function will be able to send the question for the user.
input: the request info
output: the request result
*/
RequestResult GameRequestHandler::getQuestion(RequestInfo info)
{
    RequestResult result;
    GetQuestionResponse response;

    if (m_game->getNumOfQuestions() == m_game->getPlayerData(m_user).currentQuestionIndex)
    {
        response.status = STATUS_NEG;
    }
    else
    {
        response.question = m_game->getQuestionForUser(m_user);
        response.status = STATUS_POS;
        response.numOfQuestions = m_game->getNumOfQuestions();//getting also the number of questions and the current question
        response.questionIndex = m_game->getPlayerData(m_user).currentQuestionIndex;
        response.timeForQuestion = m_game->getAnswerTimeout();
        m_lastSentQuestion = time(nullptr);

    }
    result.response = JsonResponsePacketSerializer::serializeResponse(response);
    result.newHandler = this;
    return result;
}




/*
submitAnswer: this function will handle the submit answer request, and calculations of game data
input: the request info
output: the request result
*/
RequestResult GameRequestHandler::submitAnswer(RequestInfo info)
{
    RequestResult result;
    SubmitAnswerResponse response;
    SubmitAnswerRequest request = JsonRequestPacketDeserializer::deserializeSubmitAnswerRequest(info.buffer);
    bool isCorrect = request.answer == m_game->getQuestionForUser(m_user).correctAns;
    GameData data = m_game->getPlayerData(m_user);
    int points = 0, time = static_cast<int>(info.receivalTime - m_lastSentQuestion);
    //getting the new avarege time
    if (time > m_game->getAnswerTimeout())
    {
        time = m_game->getAnswerTimeout();//to synchronize
    }
    data.averageAnswerTime = ((data.averageAnswerTime * (data.correctAnswerCount + data.wrongAnswerCount)) + time) / (data.correctAnswerCount + data.wrongAnswerCount + 1);




    if (isCorrect)
    {
        points = calcPoints(time);
        data.correctAnswerCount++;
        data.points += points;
        if (data.streak < 10)// the streak cant be above double points
        {
            data.streak++;

        }
    }
    else
    {
        data.wrongAnswerCount++;
        data.streak = 0;
    }
    response.correctAnswer = m_game->getQuestionForUser(m_user).correctAns;
    response.points = points;
    response.status = STATUS_POS;

    data.currentQuestionIndex++;
    m_game->updateData(m_user, data);
    result.newHandler = this;
    response.streak = m_game->getPlayerData(m_user).streak;
    result.response = JsonResponsePacketSerializer::serializeResponse(response);
    return result;
}



/*
getGameResults: this function will return the game results if they are available
input: the request info
output: the request result
*/
RequestResult GameRequestHandler::getGameResults(RequestInfo info)
{
    GetGameResultsResponse response;
    RequestResult result;
    std::map<std::string, GameData> gameResult = m_game->getGameResults();
    PlayerResults p;
    for (auto itt : gameResult)
    {
        p.averageAnswerTime = itt.second.averageAnswerTime;//setting the player results with the username
        p.correctAnswerCount = itt.second.correctAnswerCount;
        p.points = itt.second.points;
        p.username = itt.first;
        p.wrongAnswerCount = itt.second.wrongAnswerCount;

        response.results.push_back(p);
    }
    
    if (response.results.size() != 0)//if its empty, then it means that there are still people in the game
    {
        response.status = STATUS_POS;
        //using sort with a lambda function
        std::sort(response.results.begin(), response.results.end(), [](PlayerResults res1, PlayerResults res2) {return res1.points > res2.points; });
    }
    else
    {
        response.status = STATUS_NEG;
    }

    result.newHandler = this;
    result.response = JsonResponsePacketSerializer::serializeResponse(response);
    return result;
}



/*
leaveGame: this function will leave the game
input: the request info
output: the request result
*/
RequestResult GameRequestHandler::leaveGame(RequestInfo info)
{
    LeaveGameResponse leaveGameResponse;
    RequestResult result;

    if (m_game->getGameResults().size() != 0)// if the game is finished, then submit the answers
    {
        m_handlerFactory->getGameManager().submitScore(m_game->getId(), m_user);
    }



    if (m_game->getPlayers().size() != 1)//making sure to close the game if you are the only one left
    {
        m_game->removePlayer(m_user);

    }
    else
    {
        m_handlerFactory->getGameManager().deleteGame(m_game->getId());
    }

    leaveGameResponse.status = STATUS_POS;

    result.newHandler = m_handlerFactory->createMenuRequestHandler(m_user);// going back to menu
    result.response = JsonResponsePacketSerializer::serializeResponse(leaveGameResponse);

    return result;
}







/*
calcPoints: this function will calculate the points of the answer
input: the time it took for the answer to arrive
output: the points earnde
*/
int GameRequestHandler::calcPoints(int time)
{
    int result = 0, fiveSecondCount = 1;
    int streak = (m_game->getPlayerData(m_user).streak);
    double timeDevRatio = 1, streakRatio = (((double)streak / 10) + 1);

    if (m_game->getQuestionForUser(m_user).difficult == EASY)
    {
        result += 250;
    }
    else if (m_game->getQuestionForUser(m_user).difficult == MEDIUM)
    {
        result += 500;
    }
    else if (m_game->getQuestionForUser(m_user).difficult == HARD)
    {
        result += 750;
    }


    while (fiveSecondCount < time)
    {
        timeDevRatio += 0.01; //each 5 seconds
        fiveSecondCount += 1;
    }
    result = result / timeDevRatio;


    result = result * streakRatio;// adding streak bounus multiplyer
    return result;
}


















