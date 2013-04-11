#include "Player.h"


Player::Player(void)
{
	position = D3DXVECTOR3(0, 0, 0);
	mesh = NULL;
	texture = NULL;
	D3DXMatrixScaling(&scaleMat, 0.1f, 0.1f, 0.1f);
	rotateAngle = 0;
}

Player::~Player(void)
{
	// Shutdown the variables
	Shutdown();
}

void Player::Initialize(IDirect3DDevice9* m_pD3DDevice)
{
	// Load the mesh
	D3DXLoadMeshFromX(L"frigate.x", D3DXMESH_MANAGED, m_pD3DDevice, 
		NULL, &materialBuff, NULL, &numMaterials, &mesh);

	D3DXMaterial = (D3DXMATERIAL*) materialBuff->GetBufferPointer();
	modelMaterial = new D3DMATERIAL9[numMaterials];
	texture = new LPDIRECT3DTEXTURE9[numMaterials];

	for(DWORD i = 0; i < numMaterials; i++)
	{
		modelMaterial[i] = D3DXMaterial[i].MatD3D;
		modelMaterial[i].Ambient.r = 0.1f;
		modelMaterial[i].Ambient.g = 0.1f;
		modelMaterial[i].Ambient.b = 0.1f;
		modelMaterial[i].Ambient.a = 1.0f;

		texture[i] = NULL;
		if(D3DXMaterial[i].pTextureFilename)
		{
			int len = 0;
			len = (int)strlen(D3DXMaterial[i].pTextureFilename) + 1;
			wchar_t *ucString = new wchar_t[len];
			mbstowcs(ucString, D3DXMaterial[i].pTextureFilename, len);
			LPCWSTR filename = (LPCWSTR)ucString;
			D3DXCreateTextureFromFile(m_pD3DDevice, filename, &texture[i]);
			delete[] ucString;
		}
	}

	// build bounding box for the mesh
	BYTE* vertices = NULL;
	mesh->LockVertexBuffer(D3DLOCK_READONLY, (LPVOID*)&vertices);
	D3DXComputeBoundingBox((D3DXVECTOR3*)vertices, mesh->GetNumVertices(),
		D3DXGetFVFVertexSize(mesh->GetFVF()), &meshBox.minPt, &meshBox.maxPt);
	mesh->UnlockVertexBuffer();
}
	
void Player::Initialize2(IDirect3DDevice9* m_pD3DDevice, LPCWSTR fileName)
{
	// Load the mesh
	D3DXLoadMeshFromX(fileName, D3DXMESH_MANAGED, m_pD3DDevice, 
		NULL, &materialBuff, NULL, &numMaterials, &mesh);

	D3DXMaterial = (D3DXMATERIAL*) materialBuff->GetBufferPointer();
	modelMaterial = new D3DMATERIAL9[numMaterials];
	texture = new LPDIRECT3DTEXTURE9[numMaterials];

	for(DWORD i = 0; i < numMaterials; i++)
	{
		modelMaterial[i] = D3DXMaterial[i].MatD3D;
		modelMaterial[i].Ambient.r = 0.1f;
		modelMaterial[i].Ambient.g = 0.1f;
		modelMaterial[i].Ambient.b = 0.1f;
		modelMaterial[i].Ambient.a = 1.0f;

		texture[i] = NULL;
		if(D3DXMaterial[i].pTextureFilename)
		{
			int len = 0;
			len = (int)strlen(D3DXMaterial[i].pTextureFilename) + 1;
			wchar_t *ucString = new wchar_t[len];
			mbstowcs(ucString, D3DXMaterial[i].pTextureFilename, len);
			LPCWSTR filename = (LPCWSTR)ucString;
			D3DXCreateTextureFromFile(m_pD3DDevice, filename, &texture[i]);
			delete[] ucString;
		}
	}

	// build bounding box for the mesh
	BYTE* vertices = NULL;
	mesh->LockVertexBuffer(D3DLOCK_READONLY, (LPVOID*)&vertices);
	D3DXComputeBoundingBox((D3DXVECTOR3*)vertices, mesh->GetNumVertices(),
		D3DXGetFVFVertexSize(mesh->GetFVF()), &meshBox.minPt, &meshBox.maxPt);
	mesh->UnlockVertexBuffer();
}

void Player::Update(float dt)
{
	// player's speed
	float speed = 10.0f;

	// player's rotation speed
	float rotateSpeed = 0.2f;

	// check if the player needs to rotate
	bool rotate = false;

	// Move player up
	if(InputManager::GetInstance()->KeyboardKeyDown(DIK_UP))
	{
		position.y += speed * dt;
		rotateAngle -= rotateSpeed;
		rotate = true;
	}

	if(InputManager::GetInstance()->KeyboardKeyDown(DIK_DOWN))
	{
		position.y -= speed * dt;
		rotateAngle += rotateSpeed;
		rotate = true;
	}

	if(InputManager::GetInstance()->KeyboardKeyDown(DIK_LEFT))
	{
		position.x -= speed * dt;
	}

	if(InputManager::GetInstance()->KeyboardKeyDown(DIK_RIGHT))
	{
		position.x += speed * dt;
	}

	if(rotate)
	{
		if(rotateAngle < -45.0f)
			rotateAngle = -45.0f;
		if(rotateAngle > 45.0f)
			rotateAngle = 45.0f;
	}
	else
	{
		if(rotateAngle < 0)
			rotateAngle += rotateSpeed;
		if(rotateAngle > 0)
			rotateAngle -= rotateSpeed;
	}

	// Set rotation matrix to rotate the player
	// Yaw = y; Pitch = x; Roll = z
	D3DXMatrixRotationYawPitchRoll(&rotateMat, 
		D3DXToRadian(180.0f), D3DXToRadian(rotateAngle), 0);

	// Set the translate matrix based on the player's current position
	D3DXMatrixTranslation(&translateMat, position.x, position.y, position.z);

	// Set the world matrix
	// Note: world = scale * rotate * translate
	worldMat = scaleMat * rotateMat* translateMat;

	// Set the collision box based on the player's world position
	//meshBox.xform(worldMat, meshBox);
}

void Player::Render(ID3DXEffect* shader)
{
	// WVPMat - World View Projection Matrix
	// WITMat - World Inverse Transverse Matrix
	D3DXMATRIX WVPMat, WITMat;

	// Set the WVPMat
	WVPMat = worldMat * Camera::GetInstance()->GetViewMat() * 
		Camera::GetInstance()->GetProjMat();

	// Set the WITMat
	D3DXMatrixInverse(&WITMat, 0, &worldMat);
	D3DXMatrixTranspose(&WITMat, &WITMat);
	
	// Set shader variables
	shader->SetMatrix("worldViewProjectionMatrix", &WVPMat);
	shader->SetMatrix("worldInverseTransposeMatrix", &WITMat);
	shader->SetMatrix("worldMatrix", &worldMat);
	shader->SetValue("eyePos", &Camera::GetInstance()->GetEyePos(), 
		sizeof(D3DXVECTOR3));

	// all of our draw calls go here
	shader->SetTechnique("tech0");
	UINT numPasses = 0;
	shader->Begin(&numPasses, 0);
	for(UINT i = 0; i < numPasses; i++)
	{
		shader->BeginPass(i);
		for(DWORD j = 0; j < numMaterials; j++)
		{
			shader->SetValue("ambientMaterial", &modelMaterial[j].Ambient, sizeof(D3DXCOLOR));
			shader->SetValue("diffuseMaterial", &modelMaterial[j].Diffuse, sizeof(D3DXCOLOR));
			shader->SetValue("specularMaterial", &modelMaterial[j].Specular, sizeof(D3DXCOLOR));
			shader->SetFloat("specularPower", modelMaterial[j].Power);
			shader->SetTexture("tex", texture[j]);
			shader->SetBool("usingTexture", true);
			shader->CommitChanges();
			mesh->DrawSubset(j);
		}
		shader->EndPass();
	}
	shader->End();
}

/// Release the object's variables
void Player::Shutdown()
{
	SAFE_RELEASE(mesh);
	if(texture)
	{
		for(DWORD i = 0; i < numMaterials; i++)
		{
			SAFE_RELEASE(texture[i]);
		}
		delete[] texture;
	}
	SAFE_RELEASE(materialBuff);
}