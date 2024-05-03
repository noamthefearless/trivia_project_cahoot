#include "Server.h"
#include "SqliteDatabase.h"
#include "LoginManager.h"
#include "RequestHandlerFactory.h"
#include <nlohmann/json.hpp>
#include "QuestionInserter.h"
#include "IDatabase.h"



int main()
{
	Server s;
	s.run();
	//QuestionInserter inserter;
	return 0;
}