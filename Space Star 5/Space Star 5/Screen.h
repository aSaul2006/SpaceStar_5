#pragma once

#include <list>
#include "Initializer.h"
#include "InputManager.h"

enum GameState
{
	TitleMenu,
	MainMenu,
	Game,
	GameOver,
	NewScore,
	HighScore,
	Tutorial,
	Exit
};

enum ScreenType
{
	TitleType,
	MainType,
	GameType,
	GameOverType,
	NewScoreType,
	HighScoreType,
	TutorialType,
	ExitType
};

class CScreen
{
protected:
	ScreenType type;
	string print;
public:
	CScreen(void);
	~CScreen(void);

	virtual void Initialize(void) = 0;
	virtual void Update(GameState& gameState, float dt) = 0;
	virtual void Render(void) = 0;
	virtual void Shutdown(void) = 0;

	ScreenType GetScreenType() {return type;}

};

