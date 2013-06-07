#pragma once

#include <sqlite3.h>

class Database
{
private:
	sqlite3 *database;
	char *filename;

public:
	Database();
	~Database();

	bool open();
	void insertScore(char *name, int score);
	bool checkForHighScore(int score);
	//sqlite3 getDatabase();
	//void removeLowScore();
	//void getHighScores(string& print);
	void close();
};