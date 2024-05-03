#pragma once
#include <string>
class User
{
public :
	User() = default;
	User(const std::string name, std::string password, std::string mail);
	std::string& getName();
	std::string& getPassword();
	std::string& getMail();
	void setName(const std::string& name);
	void setPassword(const std::string& password);
	void setMail(const std::string& mail);

private:
	std::string m_name;
	std::string m_password;
	std::string m_mail;
};

