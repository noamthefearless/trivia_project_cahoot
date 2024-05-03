#include "LoggedUser.h"

std::string& LoggedUser::getUsername()
{
    return m_username;
}

LoggedUser::LoggedUser(std::string name)
{
    m_username = name;
}



bool LoggedUser::operator<(const LoggedUser& other) const
{
    return m_username < other.m_username;
}
