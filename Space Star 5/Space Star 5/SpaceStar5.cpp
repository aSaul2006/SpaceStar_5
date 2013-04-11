#include "SpaceStar5.h"


SpaceStar5::SpaceStar5(void):
m_pD3DObject(0),
	m_pD3DDevice(0),
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

	// Create the Direct3D device using the presentation parameters above
	m_pD3DObject->CreateDevice(
		D3DADAPTER_DEFAULT,		// Primary Display Device
		D3DDEVTYPE_HAL,			// Device type to use, use HAL
		hWnd,					// handle to the focus window
		D3DCREATE_HARDWARE_VERTEXPROCESSING,	// hardware behavior flags
		&D3Dpp,					// presentation parameters
		&m_pD3DDevice);			// return created D3DDevice

	// Create your sprite COM object
	D3DXCreateSprite(m_pD3DDevice, &m_pD3DSprite);

	// Initialize the Audio Manager
	AudioManager::GetInstance()->Initialize();

	// Initialize the Input Manager
	InputManager::GetInstance()->Initialize(hWnd, hInstance);

	// Initialize the Camera
	Camera::GetInstance()->Initialize(screenWidth, screenHeight);

	// Initialize the player
	player.Initialize(m_pD3DDevice);
	


	//////////////////////////////////////////////////////////////////////////////
	//// Camera Test - Initialize Object for testing
	//////////////////////////////////////////////////////////////////////////////
	D3DXCreateEffectFromFile(m_pD3DDevice, L"lab5.fx", 0, 0, 0, 0, 
		&shader, &errorCheck);

	if(errorCheck)
	{
		MessageBoxA(0,(char*)errorCheck->GetBufferPointer(), 0, 0);
	}

	SAFE_RELEASE(errorCheck);

	// Create Skybox
	skybox.BuildSkybox(m_pD3DDevice, (float)screenWidth, (float)screenHeight);

	// initialize enemy
	enemy.initializeEnemyShip(m_pD3DDevice, L"viperShip.x");

	// initialize dummy ship for collision testing
	dummyShip.Initialize(m_pD3DDevice);
	dummyShip.SetPosition(D3DXVECTOR3(5.0f, 0.0f, 0.0f));
}

/// Update Game
void SpaceStar5::Update(float dt)
{
	InputManager::GetInstance()->Update();
	Camera::GetInstance()->Update(dt);
	player.Update(dt);
	dummyShip.Update(dt);

	if(InputManager::GetInstance()->KeyboardKeyPressed(DIK_SPACE))
	{
		pList.push_back(new Projectile());
		pList.back()->SetPosition(player.GetPosition());
		pList.back()->SetStartPosition(player.GetPosition());
		pList.back()->SetDirection(D3DXVECTOR3(10.0f, 0, 0));
		pList.back()->Initialize(m_pD3DDevice);
	}

	if(pList.size() > 0)
	{
		for(list<Projectile*>::const_iterator i = pList.begin(), end = pList.end(); i != end;)
		{
			(*i)->Update(dt);

			// testing collision
			if((*i)->GetMeshBox().Intersects(dummyShip.GetMeshBox()))
			{
				(*i)->Destroy();
			}

			if((*i)->CheckObject())
			{
				delete (*i);
				i = pList.erase(i);
			}
			else
				i++;
		}
	}
}

/// Render Game
void SpaceStar5::Render()
{
	// if D3DDevice was not successfully created, don't render anything
	if(!m_pD3DDevice)
		return;

	// all draw calls between the swap chain's function
	// clear the back buffer, begin the sceen, /* draw */, EndScene, Present
	m_pD3DDevice->Clear(0, 0, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER | D3DCLEAR_STENCIL, D3DXCOLOR(0.0f, 0.4f, 0.8f, 1.0f), 1.0f, 0);

	m_pD3DDevice->BeginScene();

	// Enter rendering code here

	// Set Z render state to false to ignore Z-coordinates when
	// rendering the sky box
	m_pD3DDevice->SetRenderState(D3DRS_ZWRITEENABLE, false);

	// Render the sky box
	skybox.Render(m_pD3DDevice);

	// Set Z render state to true when finishing rendering 
	// the sky box
	m_pD3DDevice->SetRenderState(D3DRS_ZWRITEENABLE, true);

	// Render player ship
	player.Render(shader);

	// render enemy
	enemy.Render(shader);

	// render dummy ship
	dummyShip.Render(shader);

	// Render projectiles
	if(pList.size() > 0)
	{
		for(list<Projectile*>::const_iterator i = pList.begin(), end = pList.end(); i != end; i++)
		{
			(*i)->Render(shader);
		}
	}

	m_pD3DDevice->EndScene();
	m_pD3DDevice->Present(0, 0, 0, 0);
}

/// Deletes all dynamic memory created within
/// the program
void SpaceStar5::Shutdown()
{
	// Shutdown COM objects in the revrse order they were created in
	SAFE_RELEASE(m_pD3DSprite);
	SAFE_RELEASE(m_pD3DDevice);
	SAFE_RELEASE(m_pD3DObject);

	// Delete Test variables
	SAFE_RELEASE(shader);

	if(pList.size() > 0)
	{
		for(list<Projectile*>::const_iterator i = pList.begin(), end = pList.end(); i != end; )
		{
			delete (*i);
			i = pList.erase(i);
		}
	}
}