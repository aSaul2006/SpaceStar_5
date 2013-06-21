#include "Initializer.h"

Initializer* Initializer::instance;

Initializer::Initializer(void)
{
	m_pD3DDevice = NULL;
	gameFont = NULL;
	finalScore = 0;
	isOnNewScoreScreen = false;
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
	//create projectile meshes
	defaultBulletMesh.Init(L"sphere.x");
	missileMesh1.Init(L"missile1.X");
	//create enemy ship meshes
	viperMesh.Init(L"viperShip.x");
	scooterMesh.Init(L"tooter.X");
	fighterMesh.Init(L"fighterShip.X");
	//create item actor meshes
	missilePickup1.Init(L"missileItemActor.X");
	healthPickup.Init(L"healthItemActor.X");
	starDust.Init(L"starDustItemActor.X");
	//create blast mesh
	//blastMesh1.Init(L"blastMesh1.X");

	// initialize decl
	D3DVERTEXELEMENT9 particleElements[] =
	{
		{0, 0,  D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0},
		{0, 12, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 0},
		{0, 24, D3DDECLTYPE_FLOAT1, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 1},
		{0, 28, D3DDECLTYPE_FLOAT1, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 2},
		{0, 32, D3DDECLTYPE_FLOAT1, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 3},
		{0, 36, D3DDECLTYPE_FLOAT1, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 4},
		{0, 40, D3DDECLTYPE_D3DCOLOR, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_COLOR, 0},
		D3DDECL_END()
	};

	m_pD3DDevice->CreateVertexDeclaration(particleElements, &Decl);

	// Create PSystem Texture
	D3DXCreateTextureFromFileA(Initializer::GetInstance()->GetDevice(),
		"torch.dds", &mPSysTex);

	// create PSys shader effect
	ID3DXBuffer* errorCheck;
	D3DXCreateEffectFromFileA(Initializer::GetInstance()->GetDevice(),
		"firering.fx", 0, 0, 0, 0, 
		&mPSysFX, &errorCheck);

	if(errorCheck)
	{
		MessageBoxA(0,(char*)errorCheck->GetBufferPointer(), 0, 0);
	}

	SAFE_RELEASE(errorCheck);
}

void Initializer::Shutdown()
{
	SAFE_RELEASE(mPSysFX);
	SAFE_RELEASE(mPSysTex);
	SAFE_RELEASE(Decl);
	fighterMesh.Shutdown();
	scooterMesh.Shutdown();
	viperMesh.Shutdown();
	defaultBulletMesh.Shutdown();
	playerMesh.Shutdown();
	SAFE_RELEASE(m_pD3DSprite);
	SAFE_RELEASE(gameFont);
	SAFE_RELEASE(m_pD3DDevice);
}

void Initializer::setUserName(std::string value)
{
	if(g_userName.empty())
		g_userName = value;
	else if(g_userName.length() < INITIALMAX)
		g_userName += value;
}