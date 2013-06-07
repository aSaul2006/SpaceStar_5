#include "NewScoreScreen.h"
#include <stdio.h>


NewScoreScreen::NewScoreScreen()
{
	print = "";
	finalScore = Initializer::GetInstance()->getfinalscore();
	database = new Database();
	Initialize();
}

NewScoreScreen::~NewScoreScreen(void)
{
	Shutdown();
}

void NewScoreScreen::Initialize(void)
{
	D3DXCreateTextureFromFileEx(Initializer::GetInstance()->GetDevice(),
		L"titlepic.png", 800, 600, 0, 0, D3DFMT_UNKNOWN, D3DPOOL_MANAGED,
		D3DX_DEFAULT, D3DX_DEFAULT, NULL, NULL, NULL, &bgTex);
}

void NewScoreScreen::Update(GameState& gameState, float dt)
{
	//add code here for user to input initials then move to leaderboard screen
	//------------------------------------------------------------------------
	

	
	//temporary screen switch
	if(InputManager::GetInstance()->KeyboardKeyPressed(DIK_RETURN))
	{
		//database->insertScore("ZMF", finalScore); // for testing until user initials input is added
		gameState = MainMenu;
	}

}

void NewScoreScreen::Render(void)
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

	print = "New High Score!\n" + score +
			"\n\nEnter Your Initials";

	SetRect(&rect, 350, 400, 450, 600);
	fontColor = D3DCOLOR_RGBA(192, 192, 192, 255);

	//draw text
	Initializer::GetInstance()->GetFont()->DrawTextA(0, print.c_str(), -1, &rect,
		DT_CENTER | DT_NOCLIP, fontColor);

	

}

void NewScoreScreen::Shutdown(void)
{
	SAFE_RELEASE(bgTex);
}

