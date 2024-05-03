#include "LoginRequestHandler.h"


/*
isRequestRelevant: this function will check if the code in the request matchs the login and signup codes
input: the request info struct
output: a bool saying if the code is valid or not
*/
bool LoginRequestHandler::isRequestRelevant(RequestInfo reqInfo)
{
	if (reqInfo.requestCode == LOGIN || reqInfo.requestCode == SIGN_UP)
	{
		return true;
	}
	return false;
}





/*
C'tor: this function will initiallize the login Request with the factory inside
input: the factory
output: non
*/
LoginRequestHandler::LoginRequestHandler(RequestHandlerFactory* factory)
{
	m_handlerFactory = factory;
}









/*
handleRequest: this function will get the request info, and based on the request, return a request result with a response
input: the request info with the buffer, code, and timestamp of the request
output: the request result: a struct with the response buffer and a pointer to the next handler
*/
RequestResult LoginRequestHandler::handleRequest(RequestInfo reqInfo)
{
	ErrorResponse err;
	RequestResult errCase;
	try
	{
		if (isRequestRelevant(reqInfo))
		{
			if (reqInfo.requestCode == LOGIN)
			{
				return login(reqInfo);
			}
			else if (reqInfo.requestCode == SIGN_UP)
			{
				return signup(reqInfo);
			}
		}
		else
		{
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
}





/*
login: this function will attempt to login into the db, and return thr valid requestResult
input: the request info struct
the result of the request
*/
RequestResult LoginRequestHandler::login(RequestInfo reqInfo)
{
	RequestResult result;
	LoginResponse response;
	LoginRequest request;
	request = JsonRequestPacketDeserializer::DeserializeLoginRequest(reqInfo.buffer);//getting the data
	if (m_handlerFactory->getLoginManager().login(request.userName, request.password))//trying to login into the db
	{
		response.status = STATUS_POS;
		result.newHandler = m_handlerFactory->createMenuRequestHandler(LoggedUser(request.userName));//creating the next handler
	}
	else
	{
		response.status = STATUS_NEG;//if it wasn't able to connect
		result.newHandler = this;// giving back the same handler to try again
	}
	result.response = JsonResponsePacketSerializer::serializeResponse(response);//seriallizing new data
	return result;
}






/*
signup: this function will attempt to signUp into the db, and return thr valid requestResult
input: the request info struct
the result of the request
*/
RequestResult LoginRequestHandler::signup(RequestInfo reqInfo)
{
	RequestResult result;
	SignupResponse response;
	signUpRequest request;
	request = JsonRequestPacketDeserializer::DeserializeSignUpRequest(reqInfo.buffer);//getting the data
	if (m_handlerFactory->getLoginManager().signup(request.userName, request.password, request.email))//trying to signup into the db
	{
		response.status = STATUS_POS;
		result.newHandler = m_handlerFactory->createMenuRequestHandler(LoggedUser(request.userName));//creating the next handler
	}
	else
	{
		response.status = STATUS_NEG;//if it wasn't able to signUp
		result.newHandler = m_handlerFactory->createLoginRequestHandler();// giving back the same handler to try again
	}
	result.response = JsonResponsePacketSerializer::serializeResponse(response);//seriallizing new data
	return result;
}















