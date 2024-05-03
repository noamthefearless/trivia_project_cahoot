#include "User.h"

User::User(const std::string name, std::string password, std::string mail) : m_name(name), m_password(password), m_mail(mail)
{
	//Left empty
}


std::string& User::getName()
{
	return m_name;
}

std::string& User::getPassword()
{
	return m_password;
}

std::string& User::getMail()
{
	return m_mail;
}


void User::setName(const std::string& name)
{
	m_name = name;
}

void User::setPassword(const std::string& password)
{
	m_password = password;
}

void User::setMail(const std::string& mail)
{
	m_mail = mail;
}
