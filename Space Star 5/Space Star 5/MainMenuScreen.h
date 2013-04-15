#pragma once
#include "screen.h"
class MainMenuScreen :
	public CScreen
{
private:
	short choice;
public:
	MainMenuScreen(void);
	~MainMenuScreen(void);

	void Initialize(void);
	void Update(GameState& gameState, float dt);
	void Render(void);
	void Shutdown(void);

};

