#include "HighscoreScreen.h"
#include <stdio.h>

int rows = 0;

HighscoreScreen::HighscoreScreen()
{
	print = "";
	type = HighScoreType;
	database = new Database();
	names = new string[10];
	scores = new string[10];
	Initialize();
	getHighscores();
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
	print = "Highscores\n";

	for(int i =0;i < rows, i < 10;i++)
	{
		print += names[i] + "    " + scores[i] + "\n\n";
	}
	
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
		int index = 0;
		int res = 0;
		if(sqlite3_prepare_v2(database, "SELECT * FROM highscores ORDER BY score DESC;", -1, &statement, 0) == SQLITE_OK)
		{
			while(1)
			{
				res = sqlite3_step(statement);

				if(res == SQLITE_ROW)
				{
					names[index] = (char*)sqlite3_column_text(statement, 0);
					scores[index] = (char*)sqlite3_column_text(statement, 1);
					index ++;
					rows ++;
				}

				if(res == SQLITE_DONE || res == SQLITE_ERROR)
				{
					break;
				}
			
			}
			sqlite3_finalize(statement);
		}
	}
	sqlite3_close(database);
}