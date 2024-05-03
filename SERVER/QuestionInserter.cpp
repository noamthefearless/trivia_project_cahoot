#include "QuestionInserter.h"
#include <iostream>
using std::cout;


/*
insertQuestion: this function will get a question and will insert it into the db
input: the question
output: a bool saying if inserted
*/
bool QuestionInserter::insertQuestion(Question q)
{
	sqlite3* db;
	int res = sqlite3_open(std::string(DB_FILE_NAME).c_str(), &db);// opening db for insert
	std::string comm = "INSERT INTO QUESTIONS (CATEGORY, QUESTION, FALSE1, FALSE2, FALSE3, CORRECT, DIFFICULTY) VALUES ('" + q.category + "', '" + q.question + "', '" + q.falseAns1 + "', '" + q.falseAns2 + "', '" + q.falseAns3 + "', '" + q.correctAns + "', " + std::to_string(q.difficult) + ");";
	res = sqlite3_exec(db, comm.c_str(), nullptr, nullptr, nullptr);
	if (res != SQLITE_OK)
	{
		return false;
	}
	sqlite3_close(db);
	db = nullptr;
	return true;

}





/*
goThroughAjson: this function will go through a json of questions, and add them to the db
input: the json
output: non

*/
void QuestionInserter::goThroughAjson(json j)
{
	Question q;
	int numOfInsertions = 0;
	std::string choice;
	for (int i = 0; i < j["results"].size(); i++)
	{
		q.category = j["results"][i]["category"];// extracting all of the question data
		q.question = j["results"][i]["question"];
		q.correctAns = j["results"][i]["correct_answer"];
		q.falseAns1 = j["results"][i]["incorrect_answers"][0];
		q.falseAns2 = j["results"][i]["incorrect_answers"][1];
		q.falseAns3 = j["results"][i]["incorrect_answers"][2];
		if (j["results"][i]["difficulty"] == "easy")// the difficulty codes will be as followed: 1 is easy, 2 is medium, 3 is hard
		{
			q.difficult = 1;
		}
		else if (j["results"][i]["difficulty"] == "medium")
		{
			q.difficult = 2;
		}
		else
		{
			q.difficult = 3;
		}
		cout << "question: " << q.question << ", diff: " << q.difficult << ", correct: " << q.correctAns << ", false answers: " << q.falseAns1 << ", " << q.falseAns2 << ", " << q.falseAns3 << ", category: " << q.category << "\n" << std::endl;
		if (insertQuestion(q))
		{
			numOfInsertions++;
		}



	}
	cout << "\n\nout of " << j["results"].size() << " there were " << numOfInsertions << " insertions\n\n";

}







