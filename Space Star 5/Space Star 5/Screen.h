#pragma once

#include <d3d9.h>
#include <d3dx9.h>
#include <string>
#include <list>
#include "Initializer.h"
#include "InputManager.h"

#pragma comment(lib, "d3d9.lib")
#pragma comment(lib, "d3dx9.lib")

using namespace std;

enum GameState
{
	TitleMenu,
	MainMenu,
	Game,
	Exit
};

enum ScreenType
{
	TitleType,
	MainType,
	GameType,
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

