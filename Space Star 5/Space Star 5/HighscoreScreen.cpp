#include "HighscoreScreen.h"
#include <stdio.h>

HighscoreScreen::HighscoreScreen()
{
	print = "";
	type = HighScoreType;
	names = new string[10];
	scores = new string[10];
	rows = 0;
	scoreInTenthRow = "";
	scoreStore = "";
	Initialize();
	getHighscores();

	if(rows >= 10)
		DeleteRemainingEntries();

	for(int i =0;i < rows;i++)
	{
		scoreStore += names[i] + "    " + scores[i] + "\n\n";
	}
}

HighscoreScreen::~HighscoreScreen(void)
{
	Shutdown();
}

void HighscoreScreen::Initialize(void)
{
	D3DXCreateTextureFromFileEx(Initializer::GetInstance()->GetDevice(),
		L"TDDtitlePic.png", 800, 600, 0, 0, D3DFMT_UNKNOWN, D3DPOOL_MANAGED,
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
	RECT rect,scoreRect;
	D3DCOLOR fontColor;
	print = "H\ni\ng\nh\ns\nc\no\nr\ne\ns\n";
	
	SetRect(&rect, 25, 0, 400, 600);
	SetRect(&scoreRect,300,0,800,600);
	fontColor = D3DCOLOR_RGBA(192, 192, 192, 255);

	//draw text
	Initializer::GetInstance()->GetFont()->DrawTextA(0, print.c_str(), -1, &rect,
		DT_LEFT | DT_NOCLIP, fontColor);

	//draw scores
	Initializer::GetInstance()->GetFont()->DrawTextA(0, scoreStore.c_str(), -1, &scoreRect,
		DT_LEFT | DT_NOCLIP, fontColor);
}

void HighscoreScreen::Shutdown(void)
{
	SAFE_RELEASE(bgTex);
	delete names;
	delete scores;
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

					if(rows == 10)
						scoreInTenthRow = (char*)sqlite3_column_text(statement,1);
				}

				if((res == SQLITE_DONE || res == SQLITE_ERROR) || rows == 10)
				{
					break;
				}
		
			}
			sqlite3_finalize(statement);
		}
	}
	sqlite3_close(database);
}

void HighscoreScreen::DeleteRemainingEntries()
{
	sqlite3 *database;

	if(sqlite3_open("Database/spacestarDB.sqlite", &database) == SQLITE_OK)
	{
		int res = 0;
		/*sqlite3_stmt *query;
		sqlite3_stmt *deleteEntries;*/

		std::string query = "SELECT * FROM highscores ORDER BY score DESC;";
		std::string deleteEntries = "DELETE FROM highscores WHERE score < " + scoreInTenthRow + ";";

		//start sqlite transaction block
		sqlite3_exec(database,"BEGIN",0,0,0);

		res = sqlite3_exec(database, query.c_str(), 0,0,0);
		if(res == SQLITE_OK)
		{	
			res = sqlite3_exec(database,deleteEntries.c_str(),0,0,0);
			if(res != SQLITE_OK)
				sqlite3_exec(database,"ROLLBACK",0,0,0);

		}
		else
		{
			sqlite3_exec(database,"ROLLBACK",0,0,0);
		}

		// commit all to sqlite db
		sqlite3_exec(database, "COMMIT",0,0,0);
	}
	sqlite3_close(database);
}