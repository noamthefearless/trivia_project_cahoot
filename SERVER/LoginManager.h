#pragma once
#include "SqliteDatabase.h"
#include <vector>
#include "LoggedUser.h"
class LoginManager
{
public:
	LoginManager() = default;
	LoginManager(IDatabase* myDb);
	bool signup(std::string name, std::string password, std::string mail);
	bool login(std::string name, std::string password);
	bool logout(std::string name);

private:
	IDatabase* m_database;
	std::vector<LoggedUser> m_loggedUsers;
	std::vector<LoggedUser>::iterator userNameInLoggedUsers(std::string name);
};

