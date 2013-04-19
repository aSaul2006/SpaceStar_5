#pragma once

#include "Initializer.h"

class HUD
{
private:
	IDirect3DTexture9*	healthBar[3];
	RECT				barRect[3];
	D3DXVECTOR3			barPosition[3];

	float height[3];
	float width[3];

	string score;
	string lives;
public:
	HUD(void);
	~HUD(void);

	void Initialize(void);
	void Render(float currentHealth, float maxHealth, int playerScore, short playerLives);
	void Shutdown(void);
};

