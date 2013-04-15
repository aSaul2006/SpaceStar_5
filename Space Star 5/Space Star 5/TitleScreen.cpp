#include "TitleScreen.h"
#include <stdio.h>


TitleScreen::TitleScreen(void)
{
	print = "";
	type = TitleType;
}

TitleScreen::~TitleScreen(void)
{
	Shutdown();
}

void TitleScreen::Initialize(void)
{
}

void TitleScreen::Update(GameState& gameState, float dt)
{
	if(InputManager::GetInstance()->KeyboardKeyPressed(DIK_RETURN))
	{
		gameState = MainMenu;
	}
}

void TitleScreen::Render(void)
{
	RECT rect;
	D3DCOLOR fontColor;

	// stores the output in print variable
	print = "Press Enter";
	SetRect(&rect, 350, 400, 450, 600);
	fontColor = D3DCOLOR_RGBA(192, 192, 192, 255);

	// Draw Text
	Initializer::GetInstance()->GetFont()->DrawTextA(0, print.c_str(),
		-1, &rect, 
		DT_CENTER | DT_NOCLIP,
		fontColor);
}

void TitleScreen::Shutdown(void)
{
}