#include "SpaceStar5.h"

SpaceStar5::SpaceStar5(void):
m_pD3DObject(0),
	m_bVSync(false)
{
	ZeroMemory(&m_D3DCaps, sizeof(m_D3DCaps));
}


SpaceStar5::~SpaceStar5(void)
{
}

void SpaceStar5::Init(HWND hWnd, HINSTANCE hInstance, bool bWindowed)
{
	RECT windowSizeRect;
	GetWindowRect(hWnd, &windowSizeRect);
	int screenWidth = windowSizeRect.right - windowSizeRect.left;
	int screenHeight = windowSizeRect.bottom - windowSizeRect.top;

	//Create D3D9 Object
	m_pD3DObject = Direct3DCreate9(D3D_SDK_VERSION);

	// Create the Direct3D device using the presentation parameters above
	Initializer::GetInstance()->Initialize(hWnd, m_pD3DObject, bWindowed,
		m_bVSync, screenHeight, screenWidth);

	// Initialize the Audio Manager
	AudioManager::GetInstance()->Initialize();

	// Initialize the Input Manager
	InputManager::GetInstance()->Initialize(hWnd, hInstance);

	// Initialize the Camera
	Camera::GetInstance()->Initialize(screenWidth, screenHeight);

	gameState = TitleMenu;

	// Initialize BGM
	AudioManager::GetInstance()->GetSystem()->createSound("Heroic_Minority.mp3", 
		FMOD_LOOP_NORMAL | FMOD_2D | FMOD_HARDWARE, 0, &titleBGM);

	AudioManager::GetInstance()->GetSystem()->createSound("through space.ogg", 
		FMOD_LOOP_NORMAL | FMOD_2D | FMOD_HARDWARE, 0, &gameBGM);
}

/// Update Game
void SpaceStar5::Update(float dt)
{
	InputManager::GetInstance()->Update();

	switch(gameState)
	{
	case TitleMenu:
		if(gameScreen.size() == 0)
		{
			gameScreen.push_front(new TitleScreen());
			AudioManager::GetInstance()->PlayBGM(titleBGM);
		}
		else if(gameScreen.front()->GetScreenType() != TitleType)
		{
			gameScreen.pop_front();
			gameScreen.push_front(new TitleScreen());
			AudioManager::GetInstance()->PlayBGM(titleBGM);
		}
		break;
	case MainMenu:
		if(gameScreen.front()->GetScreenType() != MainType)
		{
			gameScreen.pop_front();
			gameScreen.push_front(new MainMenuScreen());
		}
		break;
	case Game:
		if(gameScreen.front()->GetScreenType() != GameType)
		{
			gameScreen.pop_front();
			gameScreen.push_front(new GameScreen());
			AudioManager::GetInstance()->PlayBGM(gameBGM);
		}
		break;
	case GameOver:
		if(gameScreen.front()->GetScreenType()  != GameOverType)
		{
			gameScreen.pop_front();
			gameScreen.push_front(new GameOverScreen());
			AudioManager::GetInstance()->PlayBGM(titleBGM);
		}
		break;
	case Exit:
		PostQuitMessage(0);
		break;
	}

	for each (CScreen* screen in gameScreen)
	{
		screen->Update(gameState, dt);
	}
}

/// Render Game
void SpaceStar5::Render()
{
	// if D3DDevice was not successfully created, don't render anything
	if(!Initializer::GetInstance()->GetDevice())
		return;

	// all draw calls between the swap chain's function
	// clear the back buffer, begin the sceen, /* draw */, EndScene, Present
	Initializer::GetInstance()->GetDevice()->Clear(0, 0, 
		D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER | D3DCLEAR_STENCIL, 
		D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f), 1.0f, 0);

	Initializer::GetInstance()->GetDevice()->BeginScene();

	// Enter rendering code here

	for each (CScreen* screen in gameScreen)
	{
		screen->Render();
	}

	Initializer::GetInstance()->GetDevice()->EndScene();
	Initializer::GetInstance()->GetDevice()->Present(0, 0, 0, 0);
}

/// Deletes all dynamic memory created within
/// the program
void SpaceStar5::Shutdown()
{
	// Shutdown COM objects in the revrse order they were created in
	titleBGM->release();
	gameBGM->release();

	if(gameScreen.size() > 0)
	{
		for(list<CScreen*>::const_iterator i = gameScreen.begin(), 
			end = gameScreen.end(); i != end; )
		{
			delete (*i);
			i = gameScreen.erase(i);
		}
	}
	InputManager::GetInstance()->Shutdown();
	AudioManager::GetInstance()->Shutdown();
	Initializer::GetInstance()->Shutdown();
	SAFE_RELEASE(m_pD3DObject);
}