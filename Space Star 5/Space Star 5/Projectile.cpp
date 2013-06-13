#include "Projectile.h"


//-----------------------------------------------------------------------------
//************************* Abstract Projectile *******************************
//-----------------------------------------------------------------------------
/*
 *
 *
 */
//------------------------------------------------------------------------------

Projectile::Projectile(void)
{
}

Projectile::~Projectile(void)
{
}


//-----------------------------------------------------------------------------
//************************* Default Bullet Class ******************************
//-----------------------------------------------------------------------------
/*
 *
 *
 */
//------------------------------------------------------------------------------

DefaultBullet::DefaultBullet(void)
{
	position = direction = startPosition = D3DXVECTOR3(0, 0, 0);
	D3DXMatrixScaling(&scaleMat, 0.1f, 0.1f, 0.1f);
	D3DXMatrixRotationYawPitchRoll(&rotateMat, 0, 0, 0);
	projectileType = DEFAULT_BULLET;

	destroyObject = false;
}

DefaultBullet::DefaultBullet(D3DXVECTOR3 spawnPosition, D3DXVECTOR3 direction)
{
	position = startPosition = spawnPosition;
	this->direction = direction;
	D3DXMatrixScaling(&scaleMat, 0.1f, 0.1f, 0.1f);
	D3DXMatrixRotationYawPitchRoll(&rotateMat, 0, 0, 0);
	destroyObject = false;
	projectileType = DEFAULT_BULLET;

	Initialize();
}

DefaultBullet::DefaultBullet(D3DXVECTOR3 spawnPosition, D3DXVECTOR3 direction,float scale)
{
	position = startPosition = spawnPosition;
	this->direction = direction;
	D3DXMatrixScaling(&scaleMat, scale, scale, scale);
	D3DXMatrixRotationYawPitchRoll(&rotateMat, 0, 0, 0);
	projectileType = DEFAULT_BULLET;
	destroyObject = false;

	Initialize();
}

DefaultBullet::~DefaultBullet(void)
{
	Shutdown();
}

void DefaultBullet::Initialize()
{
	// build bounding box for the mesh
	BYTE* vertices = NULL;
	Initializer::GetInstance()->GetDefaultBulletMesh().mesh->LockVertexBuffer(
		D3DLOCK_READONLY, (LPVOID*)&vertices);

	D3DXComputeBoundingBox((D3DXVECTOR3*)vertices, 
		Initializer::GetInstance()->GetDefaultBulletMesh().mesh->GetNumVertices(),
		D3DXGetFVFVertexSize(
		Initializer::GetInstance()->GetDefaultBulletMesh().mesh->GetFVF()), 
		&meshBox.minPt, &meshBox.maxPt);

	Initializer::GetInstance()->GetDefaultBulletMesh().mesh->UnlockVertexBuffer();
}
	
void DefaultBullet::Update(float dt)
{
	// Update the bullet's position
	position += direction * dt;

	// Check when the bullet has traveled a certain distance
	// either on the x-asix or y-axis
	if(position.x - startPosition.x >= 10.0f ||
		position.y - startPosition.y >= 10.0f)
	{
 		destroyObject = true;
	}

	// Set the translate matrix based on the player's current position
	D3DXMatrixTranslation(&translateMat, position.x, position.y, position.z);

	// Set the world matrix
	// Note: world = scale * rotate * translate
	worldMat = scaleMat * rotateMat* translateMat;

	// Set the collision box based on the player's world position
	//meshBox.xform(worldMat, meshBox);
}

void DefaultBullet::Render(ID3DXEffect* shader)
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
		for(DWORD j = 0; j < 
			Initializer::GetInstance()->GetDefaultBulletMesh().numMaterials; j++)
		{
			shader->SetValue("ambientMaterial",
				&Initializer::GetInstance()->GetDefaultBulletMesh().modelMaterial[j].Ambient, sizeof(D3DXCOLOR));
			shader->SetValue("diffuseMaterial", 
				&Initializer::GetInstance()->GetDefaultBulletMesh().modelMaterial[j].Diffuse, sizeof(D3DXCOLOR));
			shader->SetValue("specularMaterial", 
				&Initializer::GetInstance()->GetDefaultBulletMesh().modelMaterial[j].Specular, sizeof(D3DXCOLOR));
			shader->SetFloat("specularPower", 
				Initializer::GetInstance()->GetDefaultBulletMesh().modelMaterial[j].Power);
			shader->SetTexture("tex", 
				Initializer::GetInstance()->GetDefaultBulletMesh().texture[j]);
			shader->SetBool("usingTexture", true);
			shader->CommitChanges();
			Initializer::GetInstance()->GetDefaultBulletMesh().mesh->DrawSubset(j);
		}
		shader->EndPass();
	}
	shader->End();
}

/// Release the object's variables
void DefaultBullet::Shutdown()
{
}


//-----------------------------------------------------------------------------
//************************* Missile Class *************************************
//-----------------------------------------------------------------------------
/*
 *
 *
 */
//------------------------------------------------------------------------------

Missile1::Missile1(void)
{
	position = direction = startPosition = D3DXVECTOR3(0, 0, 0);
	D3DXMatrixScaling(&scaleMat, 0.1f, 0.1f, 0.1f);
	D3DXMatrixRotationYawPitchRoll(&rotateMat, 0, 0, 0);
	m_rotateAngle = 0;
	projectileType = MISSILE1;

	destroyObject = false;
}

Missile1::Missile1(D3DXVECTOR3 spawnPosition, D3DXVECTOR3 direction)
{
	position = startPosition = spawnPosition;
	this->direction = direction;
	D3DXMatrixScaling(&scaleMat, 0.1f, 0.1f, 0.1f);
	D3DXMatrixRotationYawPitchRoll(&rotateMat, 0, 0, 0);
	destroyObject = false;
	m_rotateAngle = 0;
	projectileType = MISSILE1;

	Initialize();
}

Missile1::Missile1(D3DXVECTOR3 spawnPosition, D3DXVECTOR3 direction,float scale)
{
	position = startPosition = spawnPosition;
	this->direction = direction;
	D3DXMatrixScaling(&scaleMat, scale, scale, scale);
	D3DXMatrixRotationYawPitchRoll(&rotateMat, 0, 0,-1.55f);
	destroyObject = false;
	m_rotateAngle = 0;
	projectileType = MISSILE1;

	Initialize();
}

Missile1::~Missile1(void)
{
	Shutdown();
}

void Missile1::Initialize()
{
	// build bounding box for the mesh
	BYTE* vertices = NULL;
	Initializer::GetInstance()->GetMissileMesh1().mesh->LockVertexBuffer(
		D3DLOCK_READONLY, (LPVOID*)&vertices);

	D3DXComputeBoundingBox((D3DXVECTOR3*)vertices, 
		Initializer::GetInstance()->GetMissileMesh1().mesh->GetNumVertices(),
		D3DXGetFVFVertexSize(
		Initializer::GetInstance()->GetMissileMesh1().mesh->GetFVF()), 
		&meshBox.minPt, &meshBox.maxPt);

	Initializer::GetInstance()->GetMissileMesh1().mesh->UnlockVertexBuffer();
}
	
void Missile1::Update(float dt)
{
	// Update the bullet's position
	position += direction * dt;

	m_rotateAngle = (m_rotateAngle == 360) ? 0 : ++m_rotateAngle;

	// Check when the bullet has traveled a certain distance
	// either on the x-asix or y-axis
	if(position.x - startPosition.x >= 10.0f ||
		position.y - startPosition.y >= 10.0f)
	{
 		destroyObject = true;
	}

	D3DXMatrixRotationYawPitchRoll(&rotateMat, 
		D3DXToRadian(0.0f), D3DXToRadian(m_rotateAngle), -1.55f);

	// Set the translate matrix based on the player's current position
	D3DXMatrixTranslation(&translateMat, position.x, position.y, position.z);

	// Set the world matrix
	// Note: world = scale * rotate * translate
	worldMat = scaleMat * rotateMat* translateMat;

	// Set the collision box based on the player's world position
	//meshBox.xform(worldMat, meshBox);
}

void Missile1::Render(ID3DXEffect* shader)
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
		for(DWORD j = 0; j < 
			Initializer::GetInstance()->GetMissileMesh1().numMaterials; j++)
		{
			shader->SetValue("ambientMaterial",
				&Initializer::GetInstance()->GetMissileMesh1().modelMaterial[j].Ambient, sizeof(D3DXCOLOR));
			shader->SetValue("diffuseMaterial", 
				&Initializer::GetInstance()->GetMissileMesh1().modelMaterial[j].Diffuse, sizeof(D3DXCOLOR));
			shader->SetValue("specularMaterial", 
				&Initializer::GetInstance()->GetMissileMesh1().modelMaterial[j].Specular, sizeof(D3DXCOLOR));
			shader->SetFloat("specularPower", 
				Initializer::GetInstance()->GetMissileMesh1().modelMaterial[j].Power);
			shader->SetTexture("tex", 
				Initializer::GetInstance()->GetMissileMesh1().texture[j]);
			shader->SetBool("usingTexture", true);
			shader->CommitChanges();
			Initializer::GetInstance()->GetMissileMesh1().mesh->DrawSubset(j);
		}
		shader->EndPass();
	}
	shader->End();
}

/// Release the object's variables
void Missile1::Shutdown()
{
}