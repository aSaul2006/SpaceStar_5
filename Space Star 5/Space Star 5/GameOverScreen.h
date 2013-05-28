#pragma once

#include "Screen.h"
#include <iostream>
#include "Database\tiodbc.hpp"

//safe release
#define SAFE_RELEASE(x) if(x){x->Release(); x=0;}

class GameOverScreen : public CScreen
{
private:
	IDirect3DTexture9* bgTex;

	//Database stuff
	tiodbc::connection db_HighScoreConnect;
	tiodbc::statement createStmt, readStmt, updateStmt, deleteStmt;
public:
	GameOverScreen(void);
	~GameOverScreen(void);

	void Initialize(void);
	void Update(GameState& gameState, float dt);
	void Render(void);
	void Shutdown(void);
};