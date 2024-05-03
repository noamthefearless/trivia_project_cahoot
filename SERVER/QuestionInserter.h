#pragma once
#include <nlohmann/json.hpp>
#include "IDatabase.h"
#include "SqliteDatabase.h"
using nlohmann::json;


class QuestionInserter
{
public:
	bool insertQuestion(Question q);
	void goThroughAjson(json j);
};

