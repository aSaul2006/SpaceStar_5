#pragma once

#include "Screen.h"
#include <iostream>
#include "Database.h"

//safe release
#define SAFE_RELEASE(x) if(x){x->Release(); x=0;}

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

public:
	HighscoreScreen(void);
	~HighscoreScreen(void);

	void Initialize(void);
	void Update(GameState& gameState, float dt);
	void Render(void);
	void Shutdown(void);

	void getHighscores();

};