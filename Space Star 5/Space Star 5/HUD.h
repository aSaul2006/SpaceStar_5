#pragma once

#include "Initializer.h"

class HUD
{
private:
	IDirect3DTexture9*	healthBar[3];
	RECT				barRect[3], iconRect[2];
	D3DXVECTOR3			barPosition[3];

	float height[3];
	float width[3];

	string score;
	string lives;
	string missiles1;
	string stardust;

	int timeTrack;

	RECT gaugeRect;
public:
	HUD(void);
	~HUD(void);

	void Initialize(void);
	void Render(float currentHealth, float maxHealth, int playerScore, short playerLives,
		float currentGauge, float maxGauge, short missileCount, int stardustCount);
	void Shutdown(void);
};

