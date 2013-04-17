#pragma once

#include "Screen.h"

// safe release macro
#define SAFE_RELEASE(x) if(x){x->Release(); x = 0;}

class TitleScreen : public CScreen
{
private:
	IDirect3DTexture9* bgTex;
public:
	TitleScreen(void);
	~TitleScreen(void);

	void Initialize(void);
	void Update(GameState& gameState, float dt);
	void Render(void);
	void Shutdown(void);
};

