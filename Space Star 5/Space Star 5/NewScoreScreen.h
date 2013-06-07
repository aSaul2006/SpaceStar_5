#pragma once

#include "Screen.h"
#include <iostream>
#include "Database.h"

//safe release
#define SAFE_RELEASE(x) if(x){x->Release(); x=0;}

class NewScoreScreen : public CScreen
{
private:
	IDirect3DTexture9* bgTex;
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
};