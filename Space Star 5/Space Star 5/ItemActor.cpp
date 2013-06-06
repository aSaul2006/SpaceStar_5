//ItemActor Class
// This base class will be inherited by other classes implementing
// Items dropped by enemies
#include "ItemActor.h"

ItemActor::ItemActor(void)
{

}
ItemActor::~ItemActor()
{
	Shutdown();
}

void ItemActor::Shutdown()
{
	if(itemSFX!=NULL)
	{
		itemSFX->release();
		itemSFX = NULL;
	}
}

//-----------------------------------------------------------------------------
//************************* Health Pick up class ******************************
//-----------------------------------------------------------------------------
/*
 *
 *
 */
//------------------------------------------------------------------------------
HealthItemActor::HealthItemActor(void)
{
	// Initialize variables to 0 or NULL
	D3DXMatrixScaling(&scaleMat, 1.0f, 1.0f, 1.0f);
	D3DXMatrixRotationYawPitchRoll(&rotateMat, 0, 0, 0);
	D3DXMatrixTranslation(&translateMat, 0, 0, 0);
	m_position = D3DXVECTOR3(0, 0, 0);
	m_rotateAngle = 0;
	m_speed = 0;
	this->Initialize();
}

HealthItemActor::~HealthItemActor(void)
{
}

ItemActor* HealthItemActor::DropItem(void)
{
	return new HealthItemActor();
}

void HealthItemActor::Update(float dt, baseEnemyShip* enemy)
{

}

void HealthItemActor::Render(ID3DXEffect* shader)
{
	D3DXMATRIX WVPMat, WITMat;

	// Scale, Rotate, and translate the model's worldMat
	worldMat = scaleMat * rotateMat * translateMat;

	WVPMat = worldMat * Camera::GetInstance()->GetViewMat() * 
		Camera::GetInstance()->GetProjMat();

	D3DXMatrixInverse(&WITMat, 0, &worldMat);
	D3DXMatrixTranspose(&WITMat, &WITMat);

	//shader->SetTexture("tex", player.GetTexture());
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
			Initializer::GetInstance()->GetHealthPickupMesh().numMaterials; j++)
		{
			shader->SetValue("ambientMaterial", 
				&Initializer::GetInstance()->GetHealthPickupMesh().modelMaterial[j].Ambient, sizeof(D3DXCOLOR));
			shader->SetValue("diffuseMaterial", 
				&Initializer::GetInstance()->GetHealthPickupMesh().modelMaterial[j].Diffuse, sizeof(D3DXCOLOR));
			shader->SetValue("specularMaterial", 
				&Initializer::GetInstance()->GetHealthPickupMesh().modelMaterial[j].Specular, sizeof(D3DXCOLOR));
			shader->SetFloat("specularPower", 
				Initializer::GetInstance()->GetHealthPickupMesh().modelMaterial[j].Power);
			shader->SetTexture("tex", 
				Initializer::GetInstance()->GetHealthPickupMesh().texture[j]);
			shader->SetBool("usingTexture", true);
			shader->CommitChanges();
			Initializer::GetInstance()->GetHealthPickupMesh().mesh->DrawSubset(j);
		}
		shader->EndPass();
	}
	shader->End();
}

void HealthItemActor::Initialize(void)
{
	// build bounding box for the mesh
	BYTE* vertices = NULL;
	Initializer::GetInstance()->GetHealthPickupMesh().mesh->LockVertexBuffer(
		D3DLOCK_READONLY, (LPVOID*)&vertices);

	D3DXComputeBoundingBox((D3DXVECTOR3*)vertices, 
		Initializer::GetInstance()->GetHealthPickupMesh().mesh->GetNumVertices(),
		D3DXGetFVFVertexSize(
		Initializer::GetInstance()->GetHealthPickupMesh().mesh->GetFVF()), 
		&meshBox.minPt, &meshBox.maxPt);

	Initializer::GetInstance()->GetHealthPickupMesh().mesh->UnlockVertexBuffer();

	//Add a sound later when I come up with something
	//AudioManager::GetInstance()->GetSystem()->createSound("laser3.wav",FMOD_DEFAULT,0, &itemSFX);
}


//-----------------------------------------------------------------------------
//************************* Missile 1 Pick up class ***************************
//-----------------------------------------------------------------------------
/*
 *
 *
 */
//------------------------------------------------------------------------------

Missile1ItemActor::Missile1ItemActor(void)
{
	// Initialize variables to 0 or NULL
	D3DXMatrixScaling(&scaleMat, 1.0f, 1.0f, 1.0f);
	D3DXMatrixRotationYawPitchRoll(&rotateMat, 0, 0, 0);
	D3DXMatrixTranslation(&translateMat, 0, 0, 0);
	m_position = D3DXVECTOR3(0, 0, 0);
	m_rotateAngle = 0;
	m_speed = 0;
	this->Initialize();
}

Missile1ItemActor::~Missile1ItemActor(void)
{
}

//this needs a lot of work.  I want to first make a ratio of items dropped vs. not dropped
//then in this function we determine which sort of item is dropped.  
//I would say at least 1 in 10 to 1 in 5 enemies drop an item.
ItemActor* Missile1ItemActor::DropItem(void)
{
	return new Missile1ItemActor();
}

void Missile1ItemActor::Update(float dt, baseEnemyShip* enemy)
{

}

void Missile1ItemActor::Render(ID3DXEffect* shader)
{
	D3DXMATRIX WVPMat, WITMat;

	// Scale, Rotate, and translate the model's worldMat
	worldMat = scaleMat * rotateMat * translateMat;

	WVPMat = worldMat * Camera::GetInstance()->GetViewMat() * 
		Camera::GetInstance()->GetProjMat();

	D3DXMatrixInverse(&WITMat, 0, &worldMat);
	D3DXMatrixTranspose(&WITMat, &WITMat);

	//shader->SetTexture("tex", player.GetTexture());
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
			Initializer::GetInstance()->GetMissilePickupMesh().numMaterials; j++)
		{
			shader->SetValue("ambientMaterial", 
				&Initializer::GetInstance()->GetMissilePickupMesh().modelMaterial[j].Ambient, sizeof(D3DXCOLOR));
			shader->SetValue("diffuseMaterial", 
				&Initializer::GetInstance()->GetMissilePickupMesh().modelMaterial[j].Diffuse, sizeof(D3DXCOLOR));
			shader->SetValue("specularMaterial", 
				&Initializer::GetInstance()->GetMissilePickupMesh().modelMaterial[j].Specular, sizeof(D3DXCOLOR));
			shader->SetFloat("specularPower", 
				Initializer::GetInstance()->GetMissilePickupMesh().modelMaterial[j].Power);
			shader->SetTexture("tex", 
				Initializer::GetInstance()->GetMissilePickupMesh().texture[j]);
			shader->SetBool("usingTexture", true);
			shader->CommitChanges();
			Initializer::GetInstance()->GetMissilePickupMesh().mesh->DrawSubset(j);
		}
		shader->EndPass();
	}
	shader->End();
}

void Missile1ItemActor::Initialize(void)
{
	// build bounding box for the mesh
	BYTE* vertices = NULL;
	Initializer::GetInstance()->GetMissilePickupMesh().mesh->LockVertexBuffer(
		D3DLOCK_READONLY, (LPVOID*)&vertices);

	D3DXComputeBoundingBox((D3DXVECTOR3*)vertices, 
		Initializer::GetInstance()->GetMissilePickupMesh().mesh->GetNumVertices(),
		D3DXGetFVFVertexSize(
		Initializer::GetInstance()->GetMissilePickupMesh().mesh->GetFVF()), 
		&meshBox.minPt, &meshBox.maxPt);

	Initializer::GetInstance()->GetMissilePickupMesh().mesh->UnlockVertexBuffer();

	//Add a sound later when I come up with something
	//AudioManager::GetInstance()->GetSystem()->createSound("laser3.wav",FMOD_DEFAULT,0, &itemSFX);
}
