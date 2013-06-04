#include "HUD.h"

HUD::HUD(void)
{
	healthBar[0] = NULL;	// health bar frame
	healthBar[1] = NULL;	// health bar background
	healthBar[2] = NULL;	// health bar main

	// Width and height of the health bar
	width[0]	= 400;
	height[0]	= 111;
	width[1]	= 284;
	height[1]	= 10;
	width[2]	= 284;
	height[2]	= 10;

	// RECT for each health bar
	for(short i = 0; i < 3; i++)
	{
		SetRect(&barRect[i], 0, 0, width[i], height[i]);
	}

	// Position for each health bar
	barPosition[0] = D3DXVECTOR3(0, 0, 0);
	barPosition[1] = D3DXVECTOR3(92.0f, 52.0f, 0);
	barPosition[2] = D3DXVECTOR3(92.0f, 52.0f, 0);

	score = "";

	SetRect(&gaugeRect, 0, 0, 284, 10);
}

HUD::~HUD(void)
{
	Shutdown();
}

void HUD::Initialize(void)
{
	// Initialize health bar frame
	D3DXCreateTextureFromFileEx(
		Initializer::GetInstance()->GetDevice(),
		L"health bar frame.png", width[0], height[0], 0, 0, D3DFMT_UNKNOWN, D3DPOOL_MANAGED,
		D3DX_DEFAULT, D3DX_DEFAULT, NULL, NULL, NULL, &healthBar[0]);
	
	// Initialize health bar background
	D3DXCreateTextureFromFileEx(
		Initializer::GetInstance()->GetDevice(),
		L"health bar background.png", width[1], height[1], 0, 0, D3DFMT_UNKNOWN, D3DPOOL_MANAGED,
		D3DX_DEFAULT, D3DX_DEFAULT, NULL, NULL, NULL, &healthBar[1]);

	// Initialize health bar main
	D3DXCreateTextureFromFileEx(
		Initializer::GetInstance()->GetDevice(),
		L"health bar main.png", width[2], height[2], 0, 0, D3DFMT_UNKNOWN, D3DPOOL_MANAGED,
		D3DX_DEFAULT, D3DX_DEFAULT, NULL, NULL, NULL, &healthBar[2]);
}
	
void HUD::Render(float currentHealth, float maxHealth, int playerScore, short playerLives,
	float currentGauge, float maxGauge)
{
	float remainingHealth = width[2] - (width[2] * ((maxHealth - currentHealth) / maxHealth));
	barRect[2].right = remainingHealth;

	Initializer::GetInstance()->GetSprite()->Begin(D3DXSPRITE_ALPHABLEND);

	// Render the health bar background
	Initializer::GetInstance()->GetSprite()->Draw(healthBar[1], &barRect[1], 0, 
		&barPosition[1], D3DCOLOR_ARGB(255, 255, 255, 255));

	// Render the health bar main
	Initializer::GetInstance()->GetSprite()->Draw(healthBar[2], &barRect[2], 0, 
		&barPosition[2], D3DCOLOR_ARGB(255, 255, 255, 255));

	// Render the health bar frame
	Initializer::GetInstance()->GetSprite()->Draw(healthBar[0], &barRect[0], 0, 
		&barPosition[0], D3DCOLOR_ARGB(255, 255, 255, 255));

	// Render barrel roll gauge
	float remainingGauge = width[2] - (width[2] * ((maxGauge - currentGauge) / maxGauge));
	gaugeRect.right = remainingGauge;
	D3DXVECTOR3 gaugePosition(92.0f, 75.0f, 0);
	Initializer::GetInstance()->GetSprite()->Draw(healthBar[2], &gaugeRect, 0, 
		&gaugePosition, D3DCOLOR_RGBA(0, 0, 255, 255));

	Initializer::GetInstance()->GetSprite()->End();

	// Render the score and lives
	RECT scorePos, livesPos;
	D3DCOLOR fontColor;
	fontColor = D3DCOLOR_RGBA(192, 192, 192, 255);
	SetRect(&scorePos, 600, 0, 700, 50);
	SetRect(&livesPos, 120, 10, 220, 20);
	score = "Score: " + to_string(static_cast<long long>(playerScore));
	lives = "Lives: " + to_string(static_cast<long long>(playerLives));

	// Draw score
	Initializer::GetInstance()->GetFont()->DrawTextA(0, score.c_str(),
		-1, &scorePos, 
		DT_CENTER | DT_NOCLIP,
		fontColor);

	Initializer::GetInstance()->GetFont()->DrawTextA(0, lives.c_str(),
		-1, &livesPos, 
		DT_CENTER | DT_NOCLIP,
		fontColor);

	// Render ESC message
	RECT messagePos;
	SetRect(&messagePos, 0, 550, 0, 600);
	string escMessage = "Esc - Return to Title Screen";

	Initializer::GetInstance()->GetFont()->DrawTextA(0, escMessage.c_str(),
		-1, &messagePos, 
		DT_LEFT | DT_NOCLIP,
		fontColor);
}
	
void HUD::Shutdown(void)
{
	for(short i = 0; i < 3; i++)
	{
		SAFE_RELEASE(healthBar[i]);
	}
}