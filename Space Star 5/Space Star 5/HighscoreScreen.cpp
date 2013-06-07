#include "HighscoreScreen.h"
#include <stdio.h>

HighscoreScreen::HighscoreScreen()
{
	print = "";
	database = new Database();
	names = new string[10];
	scores = new string[10];
	getHighscores();
	Initialize();
}

HighscoreScreen::~HighscoreScreen(void)
{
	Shutdown();
}

void HighscoreScreen::Initialize(void)
{
	D3DXCreateTextureFromFileEx(Initializer::GetInstance()->GetDevice(),
		L"titlepic.png", 800, 600, 0, 0, D3DFMT_UNKNOWN, D3DPOOL_MANAGED,
		D3DX_DEFAULT, D3DX_DEFAULT, NULL, NULL, NULL, &bgTex);
}

void HighscoreScreen::Update(GameState& gameState, float dt)
{
	if(InputManager::GetInstance()->KeyboardKeyPressed(DIK_RETURN))
	{
		gameState = MainMenu;
	}
}

void HighscoreScreen::Render(void)
{
	//render background
	Initializer::GetInstance()->GetSprite()->Begin(D3DXSPRITE_ALPHABLEND);
	Initializer::GetInstance()->GetSprite()->Draw(bgTex, 0, 0, 0, D3DCOLOR_ARGB(255, 255, 255, 255));
	Initializer::GetInstance()->GetSprite()->End();
	RECT rect;
	D3DCOLOR fontColor;


	
	print = "Highscores\n" + names[0] + "    " + scores[0] + "\n"
		+ names[1] + "    " + scores[1] + "\n"
		+ names[2] + "    " + scores[2] + "\n"
		+ names[3] + "    " + scores[3] + "\n"
		+ names[4] + "    " + scores[4] + "\n"
		+ names[5] + "    " + scores[5] + "\n"
		+ names[6] + "    " + scores[6] + "\n"
		+ names[7] + "    " + scores[7] + "\n"
		+ names[8] + "    " + scores[8] + "\n"
		+ names[9] + "    " + scores[9] + "\n";



	SetRect(&rect, 350, 50, 450, 600);
	fontColor = D3DCOLOR_RGBA(192, 192, 192, 255);

	//draw text
	Initializer::GetInstance()->GetFont()->DrawTextA(0, print.c_str(), -1, &rect,
		DT_CENTER | DT_NOCLIP, fontColor);
}

void HighscoreScreen::Shutdown(void)
{
	SAFE_RELEASE(bgTex);
}

void HighscoreScreen::getHighscores()
{
	sqlite3 *database;
	if(sqlite3_open("Database/spacestarDB.sqlite", &database) == SQLITE_OK)
	{
		sqlite3_stmt *statement;
		int rows = 10;
		int index = 0;
		if(sqlite3_prepare_v2(database, "SELECT * FROM highscores ORDER BY score DESC;", -1, &statement, 0) == SQLITE_OK)
		{
			while(rows > 0)
			{
				sqlite3_step(statement);
				names[index] = (char*)sqlite3_column_text(statement, 1);
				scores[index] = (char*)sqlite3_column_text(statement, 2);

				index++;
				rows--;
			}
			sqlite3_finalize(statement);
		}
	}
	sqlite3_close(database);
}