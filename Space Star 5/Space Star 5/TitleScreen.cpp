#include "TitleScreen.h"
#include <stdio.h>


TitleScreen::TitleScreen(void)
{
	print = "";
	type = TitleType;

	Initialize();
}

TitleScreen::~TitleScreen(void)
{
	Shutdown();
}

void TitleScreen::Initialize(void)
{
	D3DXCreateTextureFromFileEx(
		Initializer::GetInstance()->GetDevice(),
		L"titlepic.png", 800, 600, 0, 0, D3DFMT_UNKNOWN, D3DPOOL_MANAGED,
		D3DX_DEFAULT, D3DX_DEFAULT, NULL, NULL, NULL, &bgTex);
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
	// render the background image
	Initializer::GetInstance()->GetSprite()->Begin(D3DXSPRITE_ALPHABLEND);
	Initializer::GetInstance()->GetSprite()->Draw(bgTex, 0, 0, 0, 
		D3DCOLOR_ARGB(255, 255, 255, 255));
	Initializer::GetInstance()->GetSprite()->End();

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
	SAFE_RELEASE(bgTex);
}