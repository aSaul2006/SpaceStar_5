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

	// Presentation parameters for creating the D3D9 device
	ZeroMemory(&D3Dpp, sizeof(D3Dpp));

	D3Dpp.hDeviceWindow					= hWnd;
	D3Dpp.Windowed						= bWindowed;
	D3Dpp.AutoDepthStencilFormat		= D3DFMT_D24S8;
	D3Dpp.EnableAutoDepthStencil		= TRUE;
	D3Dpp.BackBufferCount				= 1;
	D3Dpp.BackBufferFormat				= D3DFMT_X8R8G8B8;
	D3Dpp.BackBufferHeight				= screenHeight;
	D3Dpp.BackBufferWidth				= screenWidth;
	D3Dpp.SwapEffect					= D3DSWAPEFFECT_DISCARD;
	D3Dpp.PresentationInterval			= m_bVSync ? D3DPRESENT_INTERVAL_DEFAULT : D3DPRESENT_INTERVAL_IMMEDIATE;
	D3Dpp.Flags							= D3DPRESENTFLAG_DISCARD_DEPTHSTENCIL;
	D3Dpp.FullScreen_RefreshRateInHz	= bWindowed ? 0 : D3DPRESENT_RATE_DEFAULT;
	D3Dpp.MultiSampleQuality			= 0;
	D3Dpp.MultiSampleType				= D3DMULTISAMPLE_NONE;

	//// Create the Direct3D device using the presentation parameters above
	Initializer::GetInstance()->Initialize(hWnd, m_pD3DObject, D3Dpp);

	// Initialize the Audio Manager
	AudioManager::GetInstance()->Initialize();

	// Initialize the Input Manager
	InputManager::GetInstance()->Initialize(hWnd, hInstance);

	// Initialize the Camera
	Camera::GetInstance()->Initialize(screenWidth, screenHeight);

	gameState = TitleMenu;
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
	if(gameScreen.size() > 0)
	{
		for(list<CScreen*>::const_iterator i = gameScreen.begin(), 
			end = gameScreen.end(); i != end; )
		{
			delete (*i);
			i = gameScreen.erase(i);
		}
	}
	SAFE_RELEASE(m_pD3DObject);
}