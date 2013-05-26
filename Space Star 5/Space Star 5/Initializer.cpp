#include "Initializer.h"

Initializer* Initializer::instance;

Initializer::Initializer(void)
{
	m_pD3DDevice = NULL;
	gameFont = NULL;
}

Initializer::~Initializer(void)
{
	Shutdown();
}

Initializer* Initializer::GetInstance()
{
	if(instance == 0)
		instance = new Initializer();
	return instance;
}

void Initializer::DestroyInstance()
{
	if(instance)
	{
		delete instance;
		instance = NULL;
	}
}

void Initializer::Initialize(HWND hWnd, IDirect3D9* m_pD3DObject, 
		bool bWindowed, bool m_bVSync, int screenHeight, int screenWidth)
{
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

	// Create Font COM object
	AddFontResourceEx(L"Spaceracer.ttf", FR_PRIVATE, 0);
	D3DXCreateFont(m_pD3DDevice, 30, 0, FW_BOLD, 0, false,
		DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, DEFAULT_QUALITY,
		DEFAULT_PITCH | FF_DONTCARE, TEXT("Spaceracer"),
		&gameFont);

	// Create your sprite COM object
	D3DXCreateSprite(m_pD3DDevice, &m_pD3DSprite);

	// Create player mesh
	playerMesh.Init(L"frigate.x");
	projectileMesh.Init(L"sphere.x");
	viperMesh.Init(L"viperShip.x");
	tooterMesh.Init(L"tooter.X");
}

void Initializer::Shutdown()
{
	tooterMesh.Shutdown();
	viperMesh.Shutdown();
	projectileMesh.Shutdown();
	playerMesh.Shutdown();
	SAFE_RELEASE(m_pD3DSprite);
	SAFE_RELEASE(gameFont);
	SAFE_RELEASE(m_pD3DDevice);
}