#pragma once

#include "Screen.h"
#include <iostream>
#include "Database.h"

class NewScoreScreen : public CScreen
{
private:
	IDirect3DTexture9* bgTex;
	IDirect3DTexture9* bgInputBox;
	int finalScore;
	Database *database;
	std::string m_username;

public:
	NewScoreScreen(void);
	~NewScoreScreen(void);

	void Initialize(void);
	void Update(GameState& gameState, float dt);
	void Render(void);
	void Shutdown(void);
	void InsertData(void);
};