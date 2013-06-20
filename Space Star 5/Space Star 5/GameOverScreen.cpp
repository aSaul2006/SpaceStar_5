#include "GameOverScreen.h"
#include <stdio.h>
#include "Player.h"


GameOverScreen::GameOverScreen()
{
	print = "";
	type = GameOverType;
	finalScore = Initializer::GetInstance()->getfinalscore();

	newScore = checkForHighScore(finalScore);

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
	std::string score = to_string(static_cast<long long>(finalScore));

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

bool GameOverScreen::checkForHighScore(int score)
{
	sqlite3_stmt *statement;
	int rows = 10;
	bool check = false;
	sqlite3* db;
	if(sqlite3_open("Database/spacestarDB.sqlite", &db) == SQLITE_OK)
	{
		if(sqlite3_prepare_v2(db, "SELECT * FROM highscores ORDER BY score DESC;", -1, &statement, 0) == SQLITE_OK)
		{
			int value = 0;
			while(rows > 0)
			{
				sqlite3_step(statement);
				value = sqlite3_column_int(statement, 1);
				if(score > value)
				{
					check = true;
					rows = 0;
					break;
				}
				rows--;
			}

			sqlite3_finalize(statement);
		}
	}
	return check;
}