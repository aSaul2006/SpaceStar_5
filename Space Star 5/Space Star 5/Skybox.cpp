#include "Skybox.h"


Skybox::Skybox(void)
{
}


Skybox::~Skybox(void)
{
	Shutdown();
}

void Skybox::BuildSkybox(IDirect3DDevice9* m_pD3DDevice, float screenWidth,
	float screenHeight)
{
	TVertex skyboxMesh[24] = 
	{
		// Front quad, NOTE: All quads face inward
		{-10.0f, -10.0f,  10.0f,  0.0f, 1.0f },
		{-10.0f,  10.0f,  10.0f,  0.0f, 0.0f },
		{ 10.0f, -10.0f,  10.0f,  1.0f, 1.0f },
		{ 10.0f,  10.0f,  10.0f,  1.0f, 0.0f },

		// Back quad
		{ 10.0f, -10.0f, -10.0f,  0.0f, 1.0f },
		{ 10.0f,  10.0f, -10.0f,  0.0f, 0.0f },
		{-10.0f, -10.0f, -10.0f,  1.0f, 1.0f },
		{-10.0f,  10.0f, -10.0f,  1.0f, 0.0f },

		// Left quad
		{-10.0f, -10.0f, -10.0f,  0.0f, 1.0f },
		{-10.0f,  10.0f, -10.0f,  0.0f, 0.0f },
		{-10.0f, -10.0f,  10.0f,  1.0f, 1.0f },
		{-10.0f,  10.0f,  10.0f,  1.0f, 0.0f },

		// Right quad
		{ 10.0f, -10.0f,  10.0f,  0.0f, 1.0f },
		{ 10.0f,  10.0f,  10.0f,  0.0f, 0.0f },
		{ 10.0f, -10.0f, -10.0f,  1.0f, 1.0f },
		{ 10.0f,  10.0f, -10.0f,  1.0f, 0.0f },

		// Top quad
		{-10.0f,  10.0f,  10.0f,  0.0f, 1.0f },
		{-10.0f,  10.0f, -10.0f,  0.0f, 0.0f },
		{ 10.0f,  10.0f,  10.0f,  1.0f, 1.0f },
		{ 10.0f,  10.0f, -10.0f,  1.0f, 0.0f },

		// Bottom quad
		{-10.0f, -10.0f, -10.0f,  0.0f, 1.0f },
		{-10.0f, -10.0f,  10.0f,  0.0f, 0.0f },
		{ 10.0f, -10.0f, -10.0f,  1.0f, 1.0f },
		{ 10.0f, -10.0f,  10.0f,  1.0f, 0.0f }
	};

	m_pD3DDevice->CreateVertexBuffer(sizeof(TVertex) * 24,
		0,
		FVF_FLAGS,
		D3DPOOL_MANAGED,
		&vertexBuffer,
		NULL);
	void *pVertices = NULL;
	vertexBuffer->Lock(0, sizeof(TVertex) * 24, (void**)&pVertices, 0);
	memcpy(pVertices, skyboxMesh, sizeof(TVertex) * 24);
	vertexBuffer->Unlock();

	// setting the textures for the skybox, order is important
	D3DXCreateTextureFromFile(m_pD3DDevice, L"spacey front.png" , &skyTextures[0] );
	D3DXCreateTextureFromFile(m_pD3DDevice, L"spacey back.png"  , &skyTextures[1] );
	D3DXCreateTextureFromFile(m_pD3DDevice, L"spacey left.png"  , &skyTextures[2] );
	D3DXCreateTextureFromFile(m_pD3DDevice, L"spacey right.png" , &skyTextures[3] );
	D3DXCreateTextureFromFile(m_pD3DDevice, L"spacey top.png"   , &skyTextures[4] );
	D3DXCreateTextureFromFile(m_pD3DDevice, L"spacey bottom.png", &skyTextures[5] );

	// Set up render states for the sky box
	m_pD3DDevice->SetRenderState(D3DRS_ZWRITEENABLE, false);
	m_pD3DDevice->SetRenderState(D3DRS_LIGHTING, false);

	// set up a projection matrix
	D3DXMATRIX projectionMat;
	D3DXMatrixPerspectiveFovLH(&projectionMat, D3DXToRadian(45.0f),
		screenWidth / screenHeight, 0.1f, 100.0f);
	m_pD3DDevice->SetTransform(D3DTS_PROJECTION, &projectionMat);

	D3DXMATRIX worldMat;
	D3DXMatrixIdentity(&worldMat);
	m_pD3DDevice->SetTransform(D3DTS_WORLD, &worldMat);

	m_pD3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_CLAMP);
	m_pD3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_CLAMP);
}

void Skybox::Render(IDirect3DDevice9* m_pD3DDevice)
{
	D3DXMATRIX viewMat;
	viewMat = Camera::GetInstance()->GetViewMat();

	viewMat._41 = 0.0f;
	viewMat._42 = -0.4f;
	viewMat._43 = 0.0f;

	static float rotateY = 0;

	if(InputManager::GetInstance()->KeyboardKeyDown(DIK_J))
	{
		rotateY++;
	}

	if(InputManager::GetInstance()->KeyboardKeyDown(DIK_K))
	{
		rotateY--;
	}

	D3DXMatrixRotationYawPitchRoll(&viewMat, D3DXToRadian(rotateY), 0, 0);

	m_pD3DDevice->SetTransform(D3DTS_VIEW, &viewMat);

	m_pD3DDevice->SetFVF(FVF_FLAGS);
	m_pD3DDevice->SetStreamSource(0, vertexBuffer, 0, sizeof(TVertex));

	for(UINT i = 0; i < 6; i++)
	{
		m_pD3DDevice->SetTexture(0, skyTextures[i]);
		m_pD3DDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, i * 4, 2);
	}
}

void Skybox::Shutdown()
{
	if(skyTextures)
	{
		for(DWORD i = 0; i < 6; i++)
		{
			SAFE_RELEASE(skyTextures[i]);
		}
	}
	SAFE_RELEASE(vertexBuffer);
}