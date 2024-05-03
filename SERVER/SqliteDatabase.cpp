#include "SqliteDatabase.h"
#include <iostream>

/*
getUsersCallBack: this callback function will extract the given list from the db
input: the data ptr to put the data in, the columns, rows, and column names
output: an int of 0 for seccuss
*/
int getUsersCallBack(void* data, int argc, char** argv, char** azColName)
{
    User user;
    std::list<User>* users = static_cast<std::list<User>*>(data);// casting into the user list
    for (size_t i = 0; i < argc; i++)
    {
        if (std::string(azColName[i]) == "USERNAME")
        {
            user.setName(argv[i]);
        }
        if (std::string(azColName[i]) == "PASSWORD")
        {
            user.setPassword(argv[i]);
        }
        if (std::string(azColName[i]) == "EMAIL")
        {
            user.setMail(argv[i]);
        }
        
    }
    users->push_back(user);//adding to list
    return 0;

}

/*
getUsersCallBack: this callback function will extract the given list from the db
input: the data ptr to put the data in, the columns, rows, and column names
output: an int of 0 for success
*/
int getQuestionsCallBack(void* data, int argc, char** argv, char** azColName)
{
    Question question;
    std::vector<Question>* questions = static_cast<std::vector<Question>*>(data);
    for (size_t i = 0; i < argc; i++)
    {
        if (std::string(azColName[i]) == "CATEGORY")
        {
            question.category = std::string(argv[i]);
        }
        if (std::string(azColName[i]) == "QUESTION")
        {
            question.question = std::string(argv[i]);
        }
        if (std::string(azColName[i]) == "FALSE1")
        {
            question.falseAns1 = std::string(argv[i]);
        }
        if (std::string(azColName[i]) == "FALSE2")
        {
            question.falseAns2 = std::string(argv[i]);
        }
        if (std::string(azColName[i]) == "FALSE3")
        {
            question.falseAns3 = std::string(argv[i]);
        }
        if (std::string(azColName[i]) == "DIFFICULTY")
        {
            question.difficult = std::atoi(argv[i]);
        }
        if (std::string(azColName[i]) == "CORRECT")
        {
            question.correctAns = std::string(argv[i]);
        }
    }
    questions->push_back(question);
    return 0;
}

/*
getUsersCallBack: this callback function will extract the given list from the db
input: the data ptr to put the data in, the columns, rows, and column names
output: an int of 0 for seccuss
*/
int getStatisticsCallBack(void* data, int argc, char** argv, char** azColName)
{
    Statistic statistic;
    std::list<Statistic>* statistics = static_cast<std::list<Statistic>*>(data);
    for (size_t i = 0; i < argc; i++)
    {
        if (std::string(azColName[i]) == "USERNAME")
        {
            statistic.userName = std::string(argv[i]);
        }
        if (std::string(azColName[i]) == "CORRECTANS")
        {
            statistic.correctAns = std::atoi(argv[i]);
        }
        if (std::string(azColName[i]) == "TOTALANS")
        {
            statistic.totalAns = std::atoi(argv[i]);
        }
        if (std::string(azColName[i]) == "TOTALGAMES")
        {
            statistic.totalGames = std::atoi(argv[i]);
        }
        if (std::string(azColName[i]) == "AVGTIME")
        {
            statistic.avgTime = std::atof(argv[i]);
        }
        if (std::string(azColName[i]) == "SCORE")
        {
            statistic.score = std::atoi(argv[i]);
        }
    }
    statistics->push_back(statistic);
    return 0;
}

/// <summary>
/// A  compare funciton for the sort alqorithm
/// </summary>
bool compare(const Statistic& obj1, const Statistic& obj2)
{
    return obj1.score < obj2.score;
}

SqliteDatabase::SqliteDatabase()
{
    SqliteDatabase::open();
}

SqliteDatabase::~SqliteDatabase()
{
    SqliteDatabase::close();
}

/*
open: this function will open the database
input: non
output: abool saying if it was opened or not
*/
bool SqliteDatabase::open()
{
    int file_exist = _access(std::string(DB_FILE_NAME).c_str(), 0);
    std::string command;
    int res = sqlite3_open(std::string(DB_FILE_NAME).c_str(), &m_db);
    if (res != SQLITE_OK)
    {
        m_db = nullptr;
        return false;
    }
    command = "CREATE TABLE IF NOT EXISTS USERS(USERNAME TEXT PRIMARY KEY, PASSWORD TEXT, EMAIL TEXT);";
    res = sqlite3_exec(m_db, command.c_str(), nullptr, nullptr, nullptr);//opening a table, if not exists
    command = "CREATE TABLE IF NOT EXISTS QUESTIONS(CATEGORY TEXT, QUESTION TEXT PRIMARY KEY, FALSE1 TEXT, FALSE2 TEXT, FALSE3 TEXT, CORRECT TEXT, DIFFICULTY INTEGER);";
    res = sqlite3_exec(m_db, command.c_str(), nullptr, nullptr, nullptr);//opening a table, if not exists
    command = "CREATE TABLE IF NOT EXISTS STATISTICS(USERNAME TEXT PRIMARY KEY, CORRECTANS INTEGER, TOTALANS INTEGER, TOTALGAMES INTEGER, AVGTIME REAL, SCORE INTEGER);";
    res = sqlite3_exec(m_db, command.c_str(), nullptr, nullptr, nullptr);//opening a table, if not exists
    return true;

}
/*
close: this function will close the database
input: non
output: a bool saying if closed
*/
bool SqliteDatabase::close()
{
    sqlite3_close(m_db);
    m_db = nullptr;
    return true;
}
/*
doesUserExists: this function will grt a username, and check if it exists
input: the username
output: an intiger of 1 or 0 that saying if the user exists 
*/
int SqliteDatabase::doesUserExist(std::string name)
{
    std::list<User> users = getAllUserData(name);
    return !users.empty();
}
/*
doesPasswordMatch: this function will check if a password matches a username in the db
input: the username and password
output: an intiger of 1 or 0
*/
int SqliteDatabase::doesPasswordMatch(std::string name, std::string password)
{
    std::list<User> users = getAllUserData(name);

    if (!users.empty() && users.front().getPassword() == password)//checking if matches
    {
        return 1;
    }
    return 0;
}



/*
addNewUser: this function will insert a user to the db
input: the name, password, and mail of the new user
output: a 1 or 0
*/
int SqliteDatabase::addNewUser(std::string name, std::string password, std::string mail)
{
    std::string sqlStatement = "INSERT INTO USERS (USERNAME, PASSWORD, EMAIL) VALUES ('" + name + "', '" + password + "', '" + mail + "');";
    std::string statisticsInsertStatement = "INSERT INTO STATISTICS (USERNAME, CORRECTANS, TOTALANS, TOTALGAMES, AVGTIME, SCORE) VALUES ('" + name + "', 0, 0, 0, 0, 0);";
    char* errMessage = nullptr;
    int res = sqlite3_exec(m_db, sqlStatement.c_str(), nullptr, nullptr, &errMessage);//inserting
    if (res != SQLITE_OK)
    {
        std::cout << errMessage << std::endl;
        return 0;
    }
    res = sqlite3_exec(m_db, statisticsInsertStatement.c_str(), nullptr, nullptr, &errMessage);//inserting
    if (res != SQLITE_OK)
    {
        return 0;
    }
    return 1;
}




/*
getQuestions: this function will return a list of questions based on the input:
input: a vector for the difficulties of the questions, a vector of all categories, and a number of the questions
output: a list of questions
*/
std::vector<Question> SqliteDatabase::getQuestions(std::vector<int> difficulties, std::vector<std::string>categories, int numberOfQuestions)
{
    std::string sqlStatement = "SELECT * FROM QUESTIONS WHERE (", categoriesList = "";
    char* errMessage = nullptr;
    std::vector<Question> questions;
    

    for (int k = 0; k < difficulties.size(); k++)// going through all of the difficulties requested
    {
        sqlStatement += "DIFFICULTY = " + std::to_string(difficulties[k]);
        if (k != difficulties.size() - 1)//checking if there is another one to add
        {
            sqlStatement += " OR ";
        }
    }
    categoriesList += ") AND (";// adding and for the categories
    for (int i = 0; i < categories.size(); i++)
    {
        categoriesList += "CATEGORY = \"" + categories[i] + "\"";
        if (i != categories.size() - 1)
        {
            categoriesList += " OR ";//same thing
        }
    }
    categoriesList += ")";//closing
    
    sqlStatement += categoriesList + " ORDER BY RANDOM() LIMIT " + std::to_string(numberOfQuestions) + ";";//making the order randomized
    int res = sqlite3_exec(m_db, sqlStatement.c_str(), getQuestionsCallBack, &questions, &errMessage);
    return questions;
}

/// <summary>
/// This function get the avg time per question of the user
/// </summary>
/// <param name="name">The user's name</param>
/// <returns>The avg time per question</returns>
float SqliteDatabase::getPlayerAvarageAnswerTime(std::string name)
{
    std::list<Statistic> statistics = getAllStats(name);

    if (!statistics.empty())
    {
        return statistics.front().avgTime;
    }
    return 0.0f;
}

/// <summary>
/// This function get the total answers that the user had answered correctly
/// </summary>
/// <param name="name">The name of the user</param>
/// <returns>The number of total answers the user had answered correctly</returns>
int SqliteDatabase::getNumOfCorrectAnswers(std::string name)
{
    std::list<Statistic> statistics = getAllStats(name);

    if (!statistics.empty())
    {
        return statistics.front().correctAns;
    }
    return 0;
}

/// <summary>
/// This function get the total answers that the user had answered
/// </summary>
/// <param name="name">The name of the user</param>
/// <returns>The number of total answers the user had answered</returns>
int SqliteDatabase::getNumOfTotalAnswers(std::string name)
{
    std::list<Statistic> statistics = getAllStats(name);

    if (!statistics.empty())
    {
        return statistics.front().totalAns;
    }
    return 0;
}

/// <summary>
/// This function get the number of all player's games since signin
/// </summary>
/// <param name="name">The name of the player</param>
/// <returns>The number of games</returns>
int SqliteDatabase::getNumOfPlayerGames(std::string name)
{
    std::list<Statistic> statistics = getAllStats(name);
    if (!statistics.empty())
    {
        return statistics.front().totalGames;
    }
    return 0;
}

/// <summary>
/// This functuion get the player's score
/// </summary>
/// <param name="name">The name of the player</param>
/// <returns>The score</returns>
int SqliteDatabase::getPlayerScore(std::string name)
{
    std::list<Statistic> statistics = getAllStats(name);
    if (!statistics.empty())
    {
        return statistics.front().score;
    }
    return 0;
}

/// <summary>
/// This function gets the top 5 scores and the usernames
/// </summary>
/// <returns>the usernames of the top 5 scores</returns>
std::list<Statistic> SqliteDatabase::getHighScores()
{
    std::vector<std::string> myVec;
    int count = 0;

    std::string sqlStatement = "SELECT * FROM STATISTICS ORDER BY SCORE DESC LIMIT 5;";
    char* errMessage = nullptr;
    std::list<Statistic> statistics;
    int res = sqlite3_exec(m_db, sqlStatement.c_str(), getStatisticsCallBack, &statistics, &errMessage);
    return statistics;
}


/*
getAllStats: this function will get all of the stats from a specific user
input: the username
output: a list with the statistics struct
*/
std::list<Statistic> SqliteDatabase::getAllStats(std::string name) const
{
    std::string sqlStatement = "SELECT * FROM STATISTICS WHERE USERNAME = '" + name + "';";
    char* errMessage = nullptr;
    std::list<Statistic> statistics;
    int res = sqlite3_exec(m_db, sqlStatement.c_str(), getStatisticsCallBack, &statistics, &errMessage);
    return statistics;
}


/*
getAllUserData: this function will get all of the userData from a specific user
input: the username
output: a list with the User struct
*/
std::list<User> SqliteDatabase::getAllUserData(std::string name) const
{
    std::string sqlStatement = "SELECT * FROM USERS WHERE USERNAME = '" + name + "';";
    char* errMessage = nullptr;
    std::list<User> users;
    int res = sqlite3_exec(m_db, sqlStatement.c_str(), getUsersCallBack, &users, &errMessage);
    return users;
}





/*
submitGameStatistics: this function will submit the game statistics into the db
input: the username and the data from the game
output: an integer of 1 if seccsededd
*/
int SqliteDatabase::submitGameStatistics(GameData myData, std::string username)
{
    char* errMessage = nullptr;
    //Getting the curr data
    int avgTime = getPlayerAvarageAnswerTime(username);
    int correctAns = getNumOfCorrectAnswers(username);
    int totalAns = getNumOfTotalAnswers(username);
    int score = getPlayerScore(username);

    //Manipulating the current data
    avgTime = ((avgTime * totalAns) + (myData.averageAnswerTime * (myData.correctAnswerCount + myData.wrongAnswerCount)) / (totalAns + myData.correctAnswerCount + myData.wrongAnswerCount));
    correctAns += myData.correctAnswerCount;
    totalAns += myData.correctAnswerCount + myData.wrongAnswerCount;
    score += myData.points;

    std::string sqlStatement = "UPDATE STATISTICS SET CORRECTANS = " + std::to_string(correctAns) + ", TOTALANS = " + std::to_string(totalAns) + ", TOTALGAMES = " + std::to_string(getNumOfPlayerGames(username) + 1) + ", AVGTIME = " + std::to_string(avgTime) + ", SCORE = " + std::to_string(score) + " WHERE USERNAME = \"" + username + "\";";

    int res = sqlite3_exec(m_db, sqlStatement.c_str(), nullptr, nullptr, &errMessage);//inserting
    if (res != SQLITE_OK)
    {
        std::cout << errMessage << std::endl;
        return 0;
    }
    return 1;
}



std::string SqliteDatabase::getCorrectAnswer(std::string question)
{
    std::string sqlStatement = "SELECT * FROM QUESTIONS WHERE QUESTION = \"" + question + "\";";
    char* errMessage = nullptr;
    std::vector<Question> questions;
    int res = sqlite3_exec(m_db, sqlStatement.c_str(), getQuestionsCallBack, &questions, &errMessage);
    if (questions.size() == 1)
    {
        return questions[0].correctAns;
    }
    else
    {
        return "";
    }
}



int SqliteDatabase::getNumberOfQuestions() const
{
    std::string sqlStatement = "SELECT count() FROM QUESTIONS;";
    char* errMessage = nullptr;
    std::vector<Question> questions;
    int res = sqlite3_exec(m_db, sqlStatement.c_str(), getQuestionsCallBack, &questions, &errMessage);
    return res;
}






