#pragma once

#include <sqlite3.h>
#include <iostream>

class Database
{
private:
	sqlite3 *database;
	char *filename;

public:
	Database();
	~Database();

	bool open();
	void insertScore(std::string name, int score);
	bool checkForHighScore(int score);
	//sqlite3 getDatabase();
	//void removeLowScore();
	//void getHighScores(string& print);
	void close();
};