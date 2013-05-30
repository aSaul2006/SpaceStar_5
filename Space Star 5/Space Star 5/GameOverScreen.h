#pragma once

#include "Screen.h"
#include <iostream>
#include "GameScreen.h"

//safe release
#define SAFE_RELEASE(x) if(x){x->Release(); x=0;}

class GameOverScreen : public CScreen
{
private:
	IDirect3DTexture9* bgTex;
	GameScreen game;
	int finalScore;
	
public:
	GameOverScreen(void);
	~GameOverScreen(void);

	void Initialize(void);
	void Update(GameState& gameState, float dt);
	void Render(void);
	void Shutdown(void);



};