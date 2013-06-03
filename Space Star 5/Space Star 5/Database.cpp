#include "Database.h"
#include <iostream>


Database::Database()
{
	filename = "Database/spacestarDB.sqlite";
}

Database::~Database()
{
}

bool Database::open()
{
	if(sqlite3_open(filename, &database) == SQLITE_OK)
		return true;

	return false;
}

void Database::insertScore(char *name, int score)
{
	sqlite3_stmt *statement;
	if(sqlite3_prepare_v2(database, "INSERT INTO highscores VALUES(?,?);", -1, &statement, NULL) == SQLITE_OK)
	{
		sqlite3_bind_text(statement, 1, name, strlen(name), 0);
		sqlite3_bind_int(statement, 2, score);
		sqlite3_step(statement);
		sqlite3_finalize(statement);
	}
}

bool Database::checkForHighScore(int score)
{
	sqlite3_stmt *statement;
	int rows = 10;
	bool check = false;
	if(sqlite3_prepare_v2(database, "SELECT score FROM highscores ORDER BY score;", -1, &statement, 0) == SQLITE_OK)
	{
		int value = 0;
		while(rows > 0)
		{
			value = sqlite3_step(statement);
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

void Database::close()
{
	sqlite3_close(database);
}