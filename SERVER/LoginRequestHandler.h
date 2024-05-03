#pragma once
#include "IRequestHandler.h"
#include "RequestHandlerFactory.h"
class RequestHandlerFactory;


class LoginRequestHandler : public IRequestHandler
{
public: 
	LoginRequestHandler() = default;
	LoginRequestHandler(RequestHandlerFactory* factory);
	virtual bool isRequestRelevant(RequestInfo reqInfo);
	virtual RequestResult handleRequest(RequestInfo reqInfo);
private:
	RequestResult login(RequestInfo reqInfo);
	RequestResult signup(RequestInfo reqInfo);

	RequestHandlerFactory* m_handlerFactory;
};

