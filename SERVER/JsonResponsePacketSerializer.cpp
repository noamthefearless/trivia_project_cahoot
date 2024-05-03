#include "JsonResponsePacketSerializer.h"

/// <summary>
/// This funciton creates the buffer
/// </summary>
/// <param name="j">The json format buffer</param>
/// <param name="code">The response code</param>
/// <returns>The serialized buffer</returns>
std::vector<unsigned char> createsTheBuffer(json j, unsigned char code)
{
    int dataLen;
    std::vector<unsigned char> lenghBuff(4, 0);//Creates a 4 bytes lengh buffer
    std::vector<unsigned char> buffer;
    string jsonStr = j.dump();
    std::vector<unsigned char> dataBuff(jsonStr.begin(), jsonStr.end());//Insert into dataBuf the json data in bytes
    dataLen = dataBuff.size();
    buffer.push_back(code);
    //getting the length into the buffer
    std::memcpy(lenghBuff.data(), &dataLen, sizeof(dataLen));
    // the memcpy function adds the bytes backwords for some reason
    std::reverse(lenghBuff.begin(), lenghBuff.end());
    //inserting the length and data to result buffer
    buffer.insert(buffer.end(), lenghBuff.begin(), lenghBuff.end());
    buffer.insert(buffer.end(), dataBuff.begin(), dataBuff.end());

    return buffer;
}

std::vector<unsigned char> JsonResponsePacketSerializer::serializeResponse(ErrorResponse myErr)
{
    //Creates the json message
    json j;
    j["message"] = myErr.message;

    return createsTheBuffer(j, ERROR_CODE);
}

std::vector<unsigned char> JsonResponsePacketSerializer::serializeResponse(SignupResponse mySignup)
{
    //Creates the json message
    json j;
    j["status"] = mySignup.status;

    return createsTheBuffer(j, SIGN_UP_RESPONSE);
}

std::vector<unsigned char> JsonResponsePacketSerializer::serializeResponse(LoginResponse myLogin)
{
    //Creates the json message
    json j;
    j["status"] = myLogin.status;

    return createsTheBuffer(j, LOGIN_RESPONSE);
}

std::vector<unsigned char> JsonResponsePacketSerializer::serializeResponse(LogoutResponse myLogout)
{
    json j;
    j["status"] = myLogout.status;
    return createsTheBuffer(j, LOGOUT_RESPONSE);
}

std::vector<unsigned char> JsonResponsePacketSerializer::serializeResponse(GetRoomResponse myRoom)
{
    json j;
    j["status"] = myRoom.status;
    json array = json::array();
    for (const auto& obj : myRoom.rooms)
    {
        json groupJ;
        groupJ["id"] = obj.id;
        groupJ["name"] = obj.name;
        groupJ["maxPlayers"] = obj.maxPlayers;
        groupJ["currNumOfPlayers"] = obj.currNumOfPlayers;
        groupJ["numOfQuestionsInGame"] = obj.numOfQuestionsInGame;
        groupJ["timePerQuestion"] = obj.timePerQuestion;
        groupJ["isActive"] = obj.isActive;
        groupJ["difficulty"] = obj.difficulties;
        groupJ["category"] = obj.categories;
        array.push_back(groupJ);
    }
    j["rooms"] = array;
    return createsTheBuffer(j, GET_ROOMS_RESPONSE);
}

std::vector<unsigned char> JsonResponsePacketSerializer::serializeResponse(LeaveRoomResponse myRoom)
{
    json j;
    j["status"] = myRoom.status;
    return createsTheBuffer(j, LEAVE_ROOM_RESPONSE);
}

std::vector<unsigned char> JsonResponsePacketSerializer::serializeResponse(GetGameResultsResponse myResponse)
{
    json j;
    j["status"] = myResponse.status;
    json array = json::array();
    for (const auto& obj : myResponse.results)
    {
        json groupJ;
        groupJ["userName"] = obj.username;
        groupJ["correctAnswerCount"] = obj.correctAnswerCount;
        groupJ["wrongAnswerCount"] = obj.wrongAnswerCount;
        groupJ["averageAnswerTime"] = obj.averageAnswerTime;
        groupJ["points"] = obj.points;
        array.push_back(groupJ);
    }
    j["results"] = array;
    return createsTheBuffer(j, GET_GAME_RESULTS_RESPONSE);
}

std::vector<unsigned char> JsonResponsePacketSerializer::serializeResponse(SubmitAnswerResponse myResponse)
{
    json j;
    j["status"] = myResponse.status;
    j["correctAns"] = myResponse.correctAnswer;
    j["points"] = myResponse.points;
    j["streak"] = myResponse.streak;
    return createsTheBuffer(j, SUBMIT_ANSWER_RESPONSE);
}

std::vector<unsigned char> JsonResponsePacketSerializer::serializeResponse(GetQuestionResponse myResponse)
{
    json j;
    json question;
    j["status"] = myResponse.status;
    if (myResponse.status != STATUS_NEG)
    {
        question["category"] = myResponse.question.category;
        question["question"] = myResponse.question.question;
        question["falseAns1"] = myResponse.question.falseAns1;
        question["falseAns2"] = myResponse.question.falseAns2;
        question["falseAns3"] = myResponse.question.falseAns3;
        question["correctAns"] = myResponse.question.correctAns;
        question["difficulty"] = myResponse.question.difficult;
        j["question"] = question;
        j["numOfQuestions"] = myResponse.numOfQuestions;
        j["questionIndex"] = myResponse.questionIndex;
        j["timeForQuestion"] = myResponse.timeForQuestion;
    }



    return createsTheBuffer(j, GET_QUESTION_RESPONSE);
}

std::vector<unsigned char> JsonResponsePacketSerializer::serializeResponse(LeaveGameResponse myResponse)
{
    json j;
    j["status"] = myResponse.status;
    return createsTheBuffer(j, LEAVE_GAME_RESPONSE);
}


std::vector<unsigned char> JsonResponsePacketSerializer::serializeResponse(JoinRoomResponse myRoom)
{
    json j;
    j["status"] = myRoom.status;
    return createsTheBuffer(j, JOIN_ROOM_RESPONSE);
}

std::vector<unsigned char> JsonResponsePacketSerializer::serializeResponse(CreateRoomResponse myRoom)
{
    json j;
    j["status"] = myRoom.status;
    return createsTheBuffer(j, CREATE_ROOM_RESPONSE);
}

std::vector<unsigned char> JsonResponsePacketSerializer::serializeResponse(getHighScoreResponse myScore)
{
    json j;
    j["status"] = myScore.status;
    json array = json::array();
    for (const auto& obj : myScore.statistics)
    {
        json groupJ;
        groupJ["userName"] = obj.userName;
        groupJ["correctAns"] = obj.correctAns;
        groupJ["totalAns"] = obj.totalAns;
        groupJ["totalGames"] = obj.totalGames;
        groupJ["avgTime"] = obj.avgTime;
        groupJ["score"] = obj.score;
        
        array.push_back(groupJ);
    }
    j["statistics"] = array;
    return createsTheBuffer(j, GET_HIGH_SCORE_RESPONSE);
}

std::vector<unsigned char> JsonResponsePacketSerializer::serializeResponse(getPersonalStatusResponse myStatus)
{
    json j;
    j["status"] = myStatus.status;
    json groupJ;
    groupJ["userName"] = myStatus.statistics.userName;
    groupJ["correctAns"] = myStatus.statistics.correctAns;
    groupJ["totalAns"] = myStatus.statistics.totalAns;
    groupJ["totalGames"] = myStatus.statistics.totalGames;
    groupJ["avgTime"] = myStatus.statistics.avgTime;
    groupJ["score"] = myStatus.statistics.score;
    j["statistics"] = groupJ;
    return createsTheBuffer(j, GET_PERSONAL_STATUS_RESPONSE);
}


std::vector<unsigned char> JsonResponsePacketSerializer::serializeResponse(GetPlayersInRoomResponse myRoom)
{
    json j;
    json array = json::array();
    for (const auto& obj : myRoom.players)
    {
        array.push_back(obj);
    }
    j["players"] = array;
    return createsTheBuffer(j, GET_PLAYERS_IN_ROOM_RESPONSE);
}

std::vector<unsigned char> JsonResponsePacketSerializer::serializeResponse(CloseRoomResponse myRoom)
{
    json j;
    j["status"] = myRoom.status;
    return createsTheBuffer(j, CLOSE_ROOM_RESPONSE);
}

std::vector<unsigned char> JsonResponsePacketSerializer::serializeResponse(StartGameResponse myRoom)
{
    json j;
    j["status"] = myRoom.status;
    return createsTheBuffer(j, START_GAME_RESPONSE);
}

std::vector<unsigned char> JsonResponsePacketSerializer::serializeResponse(GetRoomStateResponse myRoom)
{
    json j;
    json array = json::array();
    j["status"] = myRoom.status;
    if (myRoom.status != STATUS_NEG)
    {
        j["hasGameBegun"] = myRoom.hasGameBegun;
        for (const auto& obj : myRoom.players)
        {
            array.push_back(obj);
        }
        j["players"] = array;
        j["questionCount"] = myRoom.questionCount;
        j["answerTimeOut"] = myRoom.answerTimeout;
    }
    return createsTheBuffer(j, GET_ROOM_STATE_RESPONSE);
}





