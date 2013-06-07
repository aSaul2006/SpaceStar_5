#include "NewScoreScreen.h"
#include <stdio.h>


NewScoreScreen::NewScoreScreen()
{
	print = "";
	m_username = "";
	finalScore = Initializer::GetInstance()->getfinalscore();
	Initializer::GetInstance()->setIsOnNewScoreScreen(true);
	database = new Database();
	type = NewScoreType;
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
	m_username = Initializer::GetInstance()->getUserName();

	
	//temporary screen switch
	if(InputManager::GetInstance()->KeyboardKeyPressed(DIK_RETURN))
	{
		//The call to insertScore isn't working.  An error gets hung up some UNIX definitions
		//Which I don't think it should be hitting at all.
		//database->insertScore(m_username, finalScore); // for testing until user initials input is added
		gameState = HighScore;
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
	char temp[128];
	itoa(finalScore,temp,10);
	std::string score = temp;

	print = "New High Score!\n\n\nEnter Your Initials";
	//print = "New High Score!\n" + score +
			//"\n\nEnter Your Initials";

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

