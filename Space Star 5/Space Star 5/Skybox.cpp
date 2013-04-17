#include "Skybox.h"


Skybox::Skybox(void)
{
}


Skybox::~Skybox(void)
{
	Shutdown();
}

void Skybox::BuildSkybox(float screenWidth, float screenHeight)
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

	Initializer::GetInstance()->GetDevice()->CreateVertexBuffer(sizeof(TVertex) * 24,
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
	D3DXCreateTextureFromFile(Initializer::GetInstance()->GetDevice(), L"spacey front.png" , &skyTextures[0] );
	D3DXCreateTextureFromFile(Initializer::GetInstance()->GetDevice(), L"spacey back.png"  , &skyTextures[1] );
	D3DXCreateTextureFromFile(Initializer::GetInstance()->GetDevice(), L"spacey left.png"  , &skyTextures[2] );
	D3DXCreateTextureFromFile(Initializer::GetInstance()->GetDevice(), L"spacey right.png" , &skyTextures[3] );
	D3DXCreateTextureFromFile(Initializer::GetInstance()->GetDevice(), L"spacey top.png"   , &skyTextures[4] );
	D3DXCreateTextureFromFile(Initializer::GetInstance()->GetDevice(), L"spacey bottom.png", &skyTextures[5] );

	// Set up render states for the sky box
	Initializer::GetInstance()->GetDevice()->SetRenderState(D3DRS_ZWRITEENABLE, false);
	Initializer::GetInstance()->GetDevice()->SetRenderState(D3DRS_LIGHTING, false);

	// set up a projection matrix
	D3DXMATRIX projectionMat;
	D3DXMatrixPerspectiveFovLH(&projectionMat, D3DXToRadian(45.0f),
		screenWidth / screenHeight, 0.1f, 100.0f);
	Initializer::GetInstance()->GetDevice()->SetTransform(D3DTS_PROJECTION, &projectionMat);

	D3DXMATRIX worldMat;
	D3DXMatrixIdentity(&worldMat);
	Initializer::GetInstance()->GetDevice()->SetTransform(D3DTS_WORLD, &worldMat);

	Initializer::GetInstance()->GetDevice()->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_CLAMP);
	Initializer::GetInstance()->GetDevice()->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_CLAMP);

	rotateY = 0;
}

void Skybox::Update(float dt)
{
	float speed = 1.0f;
	viewMat = Camera::GetInstance()->GetViewMat();

	viewMat._41 = 0.0f;
	viewMat._42 = -0.4f;
	viewMat._43 = 0.0f;

	rotateY -= speed * dt;

	D3DXMatrixRotationYawPitchRoll(&viewMat, D3DXToRadian(rotateY), 0, 0);
}

void Skybox::Render()
{
	Initializer::GetInstance()->GetDevice()->SetTransform(D3DTS_VIEW, &viewMat);

	Initializer::GetInstance()->GetDevice()->SetFVF(FVF_FLAGS);
	Initializer::GetInstance()->GetDevice()->SetStreamSource(0, vertexBuffer, 0, sizeof(TVertex));

	for(UINT i = 0; i < 6; i++)
	{
		Initializer::GetInstance()->GetDevice()->SetTexture(0, skyTextures[i]);
		Initializer::GetInstance()->GetDevice()->DrawPrimitive(D3DPT_TRIANGLESTRIP, i * 4, 2);
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