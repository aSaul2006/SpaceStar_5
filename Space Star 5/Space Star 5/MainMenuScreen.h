#pragma once
#include "screen.h"
class MainMenuScreen :
	public CScreen
{
private:
	short choice;
	IDirect3DTexture9* bgTex;
public:
	MainMenuScreen(void);
	~MainMenuScreen(void);

	void Initialize(void);
	void Update(GameState& gameState, float dt);
	void Render(void);
	void Shutdown(void);

};

