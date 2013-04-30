#include "MainMenuScreen.h"


MainMenuScreen::MainMenuScreen(void)
{
	print = "";
	type = MainType;
	choice = 0;

	Initialize();
}

MainMenuScreen::~MainMenuScreen(void)
{
	Shutdown();
}

void MainMenuScreen::Initialize(void)
{
	D3DXCreateTextureFromFileEx(
		Initializer::GetInstance()->GetDevice(),
		L"titlepic.png", 800, 600, 0, 0, D3DFMT_UNKNOWN, D3DPOOL_MANAGED,
		D3DX_DEFAULT, D3DX_DEFAULT, NULL, NULL, NULL, &bgTex);
}

void MainMenuScreen::Update(GameState& gameState, float dt)
{
	if(InputManager::GetInstance()->KeyboardKeyPressed(DIK_UP))
	{
		choice--;
	}

	if(InputManager::GetInstance()->KeyboardKeyPressed(DIK_DOWN))
	{
		choice++;
	}

	if(choice < 0) choice = 1;
	if(choice > 1) choice = 0;

	if(InputManager::GetInstance()->KeyboardKeyPressed(DIK_RETURN))
	{
		switch(choice)
		{
		case 0:
			gameState = Game;
			break;
		case 1:
			gameState = Exit;
			break;
		}
	}
}

void MainMenuScreen::Render(void)
{
	// render the background image
	Initializer::GetInstance()->GetSprite()->Begin(D3DXSPRITE_ALPHABLEND);
	Initializer::GetInstance()->GetSprite()->Draw(bgTex, 0, 0, 0, 
		D3DCOLOR_ARGB(255, 255, 255, 255));
	Initializer::GetInstance()->GetSprite()->End();

	RECT rect;
	D3DCOLOR fontColor;

	// stores the output in print variable
	print = "Play Game";
	SetRect(&rect, 350, 400, 450, 450);
	if(choice == 0) fontColor = D3DCOLOR_RGBA(255, 0, 0, 255);
	else			fontColor = D3DCOLOR_RGBA(255, 255, 255, 255);

	// Draw Text
	Initializer::GetInstance()->GetFont()->DrawTextA(0, print.c_str(),
		-1, &rect, 
		DT_CENTER | DT_NOCLIP,
		fontColor);

	// stores the output in print variable
	print = "Exit";
	SetRect(&rect, 350, 500, 450, 550);
	if(choice == 1) fontColor = D3DCOLOR_RGBA(255, 0, 0, 255);
	else			fontColor = D3DCOLOR_RGBA(255, 255, 255, 255);

	// Draw Text
	Initializer::GetInstance()->GetFont()->DrawTextA(0, print.c_str(),
		-1, &rect, 
		DT_CENTER | DT_NOCLIP,
		fontColor);
}

void MainMenuScreen::Shutdown(void)
{
	SAFE_RELEASE(bgTex);
}