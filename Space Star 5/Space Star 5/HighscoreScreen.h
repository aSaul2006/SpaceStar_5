#pragma once

#include "Screen.h"
#include <iostream>
#include "Database.h"

class HighscoreScreen : public CScreen
{
private:
	IDirect3DTexture9* bgTex;
	Database *database;
	string* names;
	string* scores;
	std::string scoreStore;
	int finalScore;
	string m_username;

	int rows;
	std::string scoreInTenthRow;

public:
	HighscoreScreen(void);
	~HighscoreScreen(void);

	void Initialize(void);
	void Update(GameState& gameState, float dt);
	void Render(void);
	void Shutdown(void);

	void getHighscores();
	void DeleteRemainingEntries();	
};