#include "GameOverScreen.h"
#include <stdio.h>
#include "Player.h"

GameOverScreen* GameOverScreen::GetInstance()
{
	static GameOverScreen instance;
	return &instance;
}

GameOverScreen::GameOverScreen(void)
{
	print = "";
	type = GameOverType;
	finalScore = 0;
	Initialize();
}

GameOverScreen::~GameOverScreen(void)
{
	Shutdown();
}

void GameOverScreen::Initialize(void)
{
	D3DXCreateTextureFromFileEx(Initializer::GetInstance()->GetDevice(),
		L"titlepic.png", 800, 600, 0, 0, D3DFMT_UNKNOWN, D3DPOOL_MANAGED,
		D3DX_DEFAULT, D3DX_DEFAULT, NULL, NULL, NULL, &bgTex);
	

}

void GameOverScreen::Update(GameState& gameState, float dt)
{
	if(InputManager::GetInstance()->KeyboardKeyPressed(DIK_RETURN))
	{
		gameState = MainMenu;
	}
}

void GameOverScreen::Render(void)
{
	//render background
	Initializer::GetInstance()->GetSprite()->Begin(D3DXSPRITE_ALPHABLEND);
	Initializer::GetInstance()->GetSprite()->Draw(bgTex, 0, 0, 0, D3DCOLOR_ARGB(255, 255, 255, 255));
	Initializer::GetInstance()->GetSprite()->End();

	RECT rect;
	D3DCOLOR fontColor;

	print = "Game Over Man";

	//store the output in print variable
	//print = "Game Over";
	SetRect(&rect, 350, 400, 450, 600);
	fontColor = D3DCOLOR_RGBA(192, 192, 192, 255);

	//draw text
	Initializer::GetInstance()->GetFont()->DrawTextA(0, print.c_str(), -1, &rect,
		DT_CENTER | DT_NOCLIP, fontColor);

}

void GameOverScreen::Shutdown(void)
{
	SAFE_RELEASE(bgTex);
}