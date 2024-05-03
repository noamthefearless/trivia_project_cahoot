#include "JsonRequestPacketDeserializer.h"


/*
DeserializeLoginRequest: this function will extract the username and password into a login request struct
input: the buffer with the json inside
output: the login request as a struct, rather than a buffer
*/
LoginRequest JsonRequestPacketDeserializer::DeserializeLoginRequest(vector<unsigned char> buffer)
{
	
	json loginJ = json::parse(string(buffer.begin(), buffer.end()));// deserialaizing into a json
	LoginRequest result;
	result.userName = loginJ["username"];
	result.password = loginJ["password"];
	return result;
}


/*
DeserializeSignUpRequest: this function will extract the username, password and email of someone who wants to sign up
input: the buffer with the json inside
output: the sign up request as a struct, rather than a buffer
*/
signUpRequest JsonRequestPacketDeserializer::DeserializeSignUpRequest(vector<unsigned char> buffer)
{
	json signUpJ = json::parse(string(buffer.begin(), buffer.end()));// deserialaizing into a json
	signUpRequest result;
	result.userName = signUpJ["username"];
	result.password = signUpJ["password"];
	result.email = signUpJ["email"];
	return result;
}





/*
DeserializeRequestInfo: this function will deserealize a request length into an int
input: the length in byte form.
output: the intiger of the length
*/
int JsonRequestPacketDeserializer::DeserializeRequsetLength(vector<unsigned char> lengthInBytes)
{
	int result = 0;
	std::reverse(lengthInBytes.begin(), lengthInBytes.end());
	memcpy(&result, lengthInBytes.data(), sizeof(result));//copying into the int
	if (result < 0)
	{
		throw std::runtime_error("Invalid Size!");
	}
	return result;
}



/// <summary>
/// This function deserialize the get players request from the user
/// </summary>
/// <param name="buffer">The data</param>
/// <returns>The request object</returns>
GetPlayersRoomRequest JsonRequestPacketDeserializer::deserializeGetPlayersRequest(vector<unsigned char> buffer)
{
	GetPlayersRoomRequest result;
	json j = json::parse(string(buffer.begin(), buffer.end()));
	result.roomId = j["id"];
	return result;
}



/// <summary>
/// This fucntion deserialize the join room request
/// </summary>
/// <param name="buffer">The data</param>
/// <returns>the request object</returns>
JoinRoomRequest JsonRequestPacketDeserializer::deserializeJoinRoomRequest(vector<unsigned char> buffer)
{
	json j = json::parse(string(buffer.begin(), buffer.end()));
	JoinRoomRequest result;
	result.roomId = j["id"];
	return result;
}

/// <summary>
/// This function deserialize the create room request
/// </summary>
/// <param name="buffer">The data</param>
/// <returns>The request object</returns>
CreateRoomRequest JsonRequestPacketDeserializer::deserializeCreateRoomRequest(vector<unsigned char> buffer)
{
	json j = json::parse(string(buffer.begin(), buffer.end()));
	CreateRoomRequest result;
	result.roomName = j["name"];
	result.questionCount = j["numOfQuestionsInGame"];
	result.maxUsers = j["maxPlayers"];
	result.answerTimeout = j["timePerQuestion"];
	for (int i = 0; i < j["difficulties"].size(); i++)
	{
		result.difficulties.push_back(j["difficulties"][i]);
	}
	for (int k = 0; k < j["categories"].size(); k++)
	{
		result.categories.push_back(j["categories"][k]);
	}
	return result;
}

/// <summary>
/// This function deserialize the answer request
/// </summary>
/// <param name="buffer">The buffer</param>
/// <returns>The request object</returns>
SubmitAnswerRequest JsonRequestPacketDeserializer::deserializeSubmitAnswerRequest(vector<unsigned char> buffer)
{
	json j = json::parse(string(buffer.begin(), buffer.end()));
	SubmitAnswerRequest result;
	result.answer = j["answer"];
	return result;
}













