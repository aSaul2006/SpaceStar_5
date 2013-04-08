#include "EnemyShip.h"
#include "Camera.h"
#include "Steering Behaviors\Graph\Path.h"


//Base enemy class.  Any enemy ship class made will derive directly from this class and override some behavior.
baseEnemyShip::~baseEnemyShip()
{
	Shutdown();
}

void baseEnemyShip::initializeEnemyShip(IDirect3DDevice9* m_pD3DDevice, LPCWSTR fileName)
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

void baseEnemyShip::Render(ID3DXEffect* shader)
{
	D3DXMATRIX worldMat, WVPMat, WITMat;

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
void baseEnemyShip::fireWeapon(int fireRate)
{

}

void baseEnemyShip::update(float dt)
{
	position.x = 100;
	position.y = 100;
	position.z = 0;
	D3DXMatrixTranslation(&translateMat, position.x, position.y, position.z);
}

void baseEnemyShip::calculateDamage()
{

}

void baseEnemyShip::setPositionAndVelocity(D3DXVECTOR3 pos, D3DXVECTOR3 vel)
{
	pos = position;
	vel = velocity;
}

void baseEnemyShip::destroyShip()
{

}

void baseEnemyShip::Shutdown()
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



//Enemy State Movement Behaviors
//attackPlayer1
attackPlayer1* attackPlayer1::Instance()
{
	static attackPlayer1 instance;
	return &instance;
}

void attackPlayer1::Enter(baseEnemyShip* pEnemyShip)
{
	//behavior for player attacking state1
	
}

bool attackPlayer1::Execute(baseEnemyShip* pEnemyShip)
{
	//Execute this state
	return true;
}

void attackPlayer1::Exit(baseEnemyShip* pEnemyShip)
{

}

bool attackPlayer1::OnMessage(baseEnemyShip* agent, const Message& msg)
{
	return true;
}

//attackPlayer2
attackPlayer2* attackPlayer2::Instance()
{
	static attackPlayer2 instance;
	return &instance;
}

void attackPlayer2::Enter(baseEnemyShip* pEnemyShip)
{
	//behavior for player attacking state1
	
}

bool attackPlayer2::Execute(baseEnemyShip* pEnemyShip)
{
	//Execute this state
	return true;
}

void attackPlayer2::Exit(baseEnemyShip* pEnemyShip)
{
	//exit the state
}

bool attackPlayer2::OnMessage(baseEnemyShip* agent, const Message& msg)
{
	return true;
}

//attackPlayer3
attackPlayer3* attackPlayer3::Instance()
{
	static attackPlayer3 instance;
	return &instance;
}

void attackPlayer3::Enter(baseEnemyShip* pEnemyShip)
{
	//behavior for player attacking state1
	
}

bool attackPlayer3::Execute(baseEnemyShip* pEnemyShip)
{
	//Execute this state
	return true;
}

void attackPlayer3::Exit(baseEnemyShip* pEnemyShip)
{

}

bool attackPlayer3::OnMessage(baseEnemyShip* agent, const Message& msg)
{
	return true;
}

//avoidPlayer1
avoidPlayer1* avoidPlayer1::Instance()
{
	static avoidPlayer1 instance;
	return &instance;
}

void avoidPlayer1::Enter(baseEnemyShip* pEnemyShip)
{
	//behavior for player avoiding state1
	
}

bool avoidPlayer1::Execute(baseEnemyShip* pEnemyShip)
{
	//Execute this state
	return true;
}

void avoidPlayer1::Exit(baseEnemyShip* pEnemyShip)
{

}

bool avoidPlayer1::OnMessage(baseEnemyShip* agent, const Message& msg)
{
	return true;
}