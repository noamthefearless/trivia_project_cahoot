#include "LoginManager.h"

LoginManager::LoginManager(IDatabase* myDb) : m_database(myDb)
{
	//Left empty
}

/// <summary>
/// This function signup the user, adds the user to the database
/// </summary>
/// <param name="name">The name of the new user</param>
/// <param name="password">The password of the new user</param>
/// <param name="mail">The mail of the new user</param>
/// <returns>If the signup was succesful</returns>
bool LoginManager::signup(std::string name, std::string password, std::string mail)
{
	if (!m_database->doesUserExist(name))
	{
		m_database->addNewUser(name, password, mail);
		m_loggedUsers.push_back(LoggedUser(name));
		return true;
	}
	return false;
}

/// <summary>
/// This function checks if the user name and the password are correct, if so the function add the user to the m_loggedUser
/// </summary>
/// <param name="name">The name of the user to be logged in</param>
/// <param name="password">The password of the user to be logged in</param>
/// <returns>If the loggin was succesful</returns>
bool LoginManager::login(std::string name, std::string password)
{
	if (m_database->doesUserExist(name) && m_database->doesPasswordMatch(name, password) && userNameInLoggedUsers(name) == m_loggedUsers.end())
	{
		m_loggedUsers.push_back(LoggedUser(name));
		return true;
	}
	return false;
}

/// <summary>
/// This function checks if the user is in the logged users, if so it erase it from there
/// </summary>
/// <param name="name">The name of the user to be logged out</param>
/// <returns>If the logout was succesful</returns>
bool LoginManager::logout(std::string name)
{
	std::vector<LoggedUser>::iterator it = userNameInLoggedUsers(name);
	if (it != m_loggedUsers.end())
	{
		m_loggedUsers.erase(it);
		return true;
	}
	return false;
}

/// <summary>
/// This function checks if a users is already logged in
/// </summary>
/// <param name="name">The user's name</param>
/// <returns>The user's iterator in loggedUser</returns>
std::vector<LoggedUser>::iterator LoginManager::userNameInLoggedUsers(std::string name)
{
	std::vector<LoggedUser>::iterator it;
	for ( it = m_loggedUsers.begin(); it != m_loggedUsers.end(); it++)
	{
		if ((*it).getUsername() == name)
		{
			return it;
		}
	}
	return m_loggedUsers.end();
}
