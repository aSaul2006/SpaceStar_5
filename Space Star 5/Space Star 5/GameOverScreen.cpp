#include "GameOverScreen.h"
#include <stdio.h>
#include "Player.h"


GameOverScreen::GameOverScreen()
{
	print = "";
	type = GameOverType;
	finalScore = Initializer::GetInstance()->getfinalscore();

	database = new Database();
	char *name = "ZMF";
	//newScore = false;

	database->open();
	newScore = database->checkForHighScore(finalScore);
	database->close();

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
		if(newScore)
			gameState= NewScore;
			//gameState = HighScore;//inserted for testing purposes
		else
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
	char temp[128];
	itoa(finalScore,temp,10);
	std::string score = temp;

	print = "Game Over Man.\nFinal Score = " + score;

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