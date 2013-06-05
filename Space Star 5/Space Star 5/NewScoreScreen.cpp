#include "NewScoreScreen.h"
#include <stdio.h>
#define TEXTMAX 10

NewScoreScreen::NewScoreScreen()
{
	print = "";
	m_username = Initializer::GetInstance()->getUserName();
	type = NewScoreType;
	finalScore = Initializer::GetInstance()->getfinalscore();
	Initialize();
	inputPosition = 0;
	Initializer::GetInstance()->setIsOnNewScoreScreen(true);
	//textBuffer[1]=('t');
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
	//if(InputManager::GetInstance()->KeyboardKeyDown())

	//temporary screen switch
	if(InputManager::GetInstance()->KeyboardKeyPressed(DIK_RETURN))
	{
		gameState = MainMenu;
	}

}

void NewScoreScreen::Render(void)
{
	//render background
	Initializer::GetInstance()->GetSprite()->Begin(D3DXSPRITE_ALPHABLEND);
	Initializer::GetInstance()->GetSprite()->Draw(bgTex, 0, 0, 0, D3DCOLOR_ARGB(255, 255, 255, 255));
	Initializer::GetInstance()->GetSprite()->End();
	RECT rect,textBox;
	D3DCOLOR fontColor;
	//char temp[128];
	//itoa(finalScore,temp,10);
	//std::string score = temp;

	print = "New High Score!\n\n\nEnter Your Initials";

	SetRect(&rect, 350, 400, 450, 600);
	SetRect(&textBox,600,500,800,600);
	fontColor = D3DCOLOR_RGBA(192, 192, 192, 255);

	//draw text
	Initializer::GetInstance()->GetFont()->DrawTextA(0, print.c_str(), -1, &rect,
		DT_CENTER | DT_NOCLIP, fontColor);
	Initializer::GetInstance()->GetFont()->DrawTextA(0,m_username.c_str(), -1, &textBox,
		DT_CENTER | DT_NOCLIP, fontColor);
	
}

void NewScoreScreen::Shutdown(void)
{
	Initializer::GetInstance()->setIsOnNewScoreScreen(false);
	SAFE_RELEASE(bgTex);
}