#pragma once
#include <string>

class LoggedUser
{
public:
	LoggedUser() = default;
	std::string& getUsername();
	LoggedUser(std::string name);
	bool operator<(const LoggedUser& other) const;
private:
	std::string m_username;
};

