#include "Database.h"
#include <iostream>
#include <Windows.h>


Database::Database()
{
	filename = "Database/spacestarDB.sqlite";
}

Database::~Database()
{
}

bool Database::open()
{
	bool retCode;
	if(sqlite3_open(filename, &database) == SQLITE_OK)
		retCode = true;

	else
		retCode = false;
	return retCode;
}

void Database::insertScore(std::string name, int score)
{
	sqlite3_stmt *statement;

	if(sqlite3_prepare_v2(database, "INSERT INTO highscores VALUES(?,?);", -1, &statement, NULL) == SQLITE_OK)
	{
		sqlite3_bind_text(statement, 0, name.c_str(), strlen(name.c_str()), 0);
		sqlite3_bind_int(statement, 1, score);
		sqlite3_step(statement);
		sqlite3_finalize(statement);
	}
	else
	{
		std::wstring msg = L"Insert Failed" ;
		MessageBox(NULL,msg.c_str(),L"DATABASE ERROR", MB_OK | MB_ICONINFORMATION);
	}
}

bool Database::checkForHighScore(int score)
{
	sqlite3_stmt *statement;
	int rows = 10;
	bool check = false;
	if(sqlite3_prepare_v2(database, "SELECT score FROM highscores ORDER BY score DESC;", -1, &statement, 0) == SQLITE_OK)
	{
		int value = 0;
		while(rows > 0)
		{
			sqlite3_step(statement);
			value = sqlite3_column_int(statement, 2);
			if(score > value)
			{
				check = true;
				rows = 0;
			}
			rows--;
		}

		sqlite3_finalize(statement);
	}
	return check;
}

/*void Database::getHighScores(string& print)
{
	sqlite3_stmt *statement;
	int rows = 10;
	int index = 0;
	if(sqlite3_prepare_v2(database, "SELECT * FROM highscores ORDER BY score DESC;", -1, &statement, 0) == SQLITE_OK)
	{
		while(rows > 0)
		{
			sqlite3_step(statement);
			names[index] = (char)sqlite3_column_text(statement, 1);
			scores[index] = (char)sqlite3_column_text(statement, 2);

			index++;
			rows--;
		}
		sqlite3_finalize(statement);
	}
}*/
/*void Database::removeLowScore()
{
	sqlite3_stmt *statement;
	int id = 0;
	if(sqlite3_prepare_v2(database, "SELECT * FROM highscores ORDER BY score;", -1, &statement, 0) == SQLITE_OK)
	{
		sqlite3_step(statement);
		id = sqlite3_column_int(statement, 0);
		sqlite3_finalize(statement);
		if(sqlite3_prepare_v2(database, "DELETE FROM highscores WHERE id=?;",  -1, &statement, 0) == SQLITE_OK)
		{
			sqlite3_bind_int(statement, 1, id);
			sqlite3_step(statement);
			sqlite3_finalize(statement);
		}

	}
}*/

void Database::close()
{
	sqlite3_close(database);
}