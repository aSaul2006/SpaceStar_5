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

	D3DXCreateTextureFromFileEx(Initializer::GetInstance()->GetDevice(),
		L"inputColor.png", 250, 30, 0, 0, D3DFMT_UNKNOWN, D3DPOOL_MANAGED,
		D3DX_DEFAULT, D3DX_DEFAULT, NULL, NULL, NULL, &bgInputBox);
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
		InsertData();
		gameState = HighScore;
	}

}

void NewScoreScreen::Render(void)
{
	D3DXVECTOR3 inputPos = D3DXVECTOR3(275,500,0);
	//render background
	Initializer::GetInstance()->GetSprite()->Begin(D3DXSPRITE_ALPHABLEND);
	Initializer::GetInstance()->GetSprite()->Draw(bgTex, 0, 0, 0, D3DCOLOR_ARGB(255, 255, 255, 255));
	Initializer::GetInstance()->GetSprite()->Draw(bgInputBox,0,0,&inputPos,D3DCOLOR_ARGB(255, 255, 255, 255));
	Initializer::GetInstance()->GetSprite()->End();

	RECT rect1,rect2,textBox;
	SetRect(&rect1, 350, 400, 450, 450);
	SetRect(&rect2, 350, 450, 450, 500);
	SetRect(&textBox, 290, 500, 450, 550);

	D3DCOLOR fontColor;

	std::string score = to_string(static_cast<long long>(finalScore));


	print = "New High Score!";
	std::string msg = "Enter Your Initials: ";
	//print = "New High Score!\n" + score +
			//"\n\nEnter Your Initials";

	fontColor = D3DCOLOR_RGBA(192, 192, 192, 255);
	
	//draw text
	Initializer::GetInstance()->GetFont()->DrawTextA(0, print.c_str(), -1, &rect1,
		DT_CENTER | DT_NOCLIP, fontColor);

	Initializer::GetInstance()->GetFont()->DrawTextA(0, msg.c_str(), -1, &rect2,
		DT_CENTER | DT_NOCLIP, fontColor);

	Initializer::GetInstance()->GetFont()->DrawTextA(0,m_username.c_str(), -1, &textBox,
	    DT_LEFT | DT_NOCLIP, D3DCOLOR_RGBA(0,0,0,255));

	

}

void NewScoreScreen::Shutdown(void)
{
	Initializer::GetInstance()->setIsOnNewScoreScreen(false);
	database->close();
	SAFE_RELEASE(bgTex);
}


void NewScoreScreen::InsertData(void)
{
	sqlite3_stmt *statement;
	sqlite3 *database;

	if(sqlite3_open("Database/spacestarDB.sqlite", &database) == SQLITE_OK)
	{
		if(sqlite3_prepare_v2(database, "INSERT INTO highscores VALUES(?,?);", -1, &statement, NULL) == SQLITE_OK)
		{
			sqlite3_bind_text(statement, 1, m_username.c_str(), strlen(m_username.c_str()), 0);
			sqlite3_bind_int(statement, 2, finalScore);
			sqlite3_step(statement);
			sqlite3_finalize(statement);
		}
		else
		{
			std::wstring msg = L"Insert Failed" ;
			MessageBox(NULL,msg.c_str(),L"DATABASE ERROR", MB_OK | MB_ICONINFORMATION);
		}
	}
}
