#include "Player.h"


Player::Player(void)
{
	position = D3DXVECTOR3(0, 0, 0);
	D3DXMatrixScaling(&scaleMat, 0.1f, 0.1f, 0.1f);
	rotateAngle = 0;
	currentHealth = 100.0f;
	maxHealth = 100.0f;
	score = 0;
	lives = 3;
	barrelRoll = false;
	rollNum = 0;
	currentGauge = 100.0f;
	maxGauge = 100.0f;
}

Player::~Player(void)
{
	// Shutdown the variables
	Shutdown();
}

void Player::Initialize()
{
	// build bounding box for the mesh
	BYTE* vertices = NULL;
	Initializer::GetInstance()->GetPlayerMesh().mesh->LockVertexBuffer(
		D3DLOCK_READONLY, (LPVOID*)&vertices);

	D3DXComputeBoundingBox((D3DXVECTOR3*)vertices, 
		Initializer::GetInstance()->GetPlayerMesh().mesh->GetNumVertices(),
		D3DXGetFVFVertexSize( 
		Initializer::GetInstance()->GetPlayerMesh().mesh->GetFVF()), 
		&meshBox.minPt, &meshBox.maxPt);

	Initializer::GetInstance()->GetPlayerMesh().mesh->UnlockVertexBuffer();

	// Initialize player's HUD
	playerHUD.Initialize();
}

void Player::Update(float dt)
{
	// check player's health and see if player has lost lives
	if(currentHealth <= 0.0f)
	{
		currentHealth = maxHealth;
		lives--;
	}

	if(!barrelRoll)
	{
		CheckPlayerInput(dt);

		if(currentGauge < 100.0f)
		{
			currentGauge += 25.0f * dt;
		}
		else if(currentGauge > 100.0f)
		{
			currentGauge = 100.0f;
		}
	}
	else
	{
		float rotateSpeed = 800.0f;

		rotateAngle += rotateSpeed * dt;
		currentGauge -= 150.0f * dt;

		if(currentGauge < 0.0f)
		{
			currentGauge = 0.0f;
		}

		if(rotateAngle >= 360.0f)
		{
			rotateAngle = 0.0f;
			rollNum++;
		}

		if(rollNum >= 2)
		{
			rotateAngle = 0.0f;
			rollNum = 0;
			barrelRoll = false;
		}
	}

	// keep up with the moving camera
	//position.x += Camera::GetInstance()->GetCameraSpeed() * dt;

	// Set rotation matrix to rotate the player
	// Yaw = y; Pitch = x; Roll = z
	D3DXMatrixRotationYawPitchRoll(&rotateMat, 
		D3DXToRadian(180.0f), D3DXToRadian(rotateAngle), 0);

	// Set the translate matrix based on the player's current position
	D3DXMatrixTranslation(&translateMat, position.x, position.y, position.z);

	// Set the world matrix
	// Note: world = scale * rotate * translate
	worldMat = scaleMat * rotateMat* translateMat;
}

void Player::CheckPlayerInput(float dt)
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
		if(position.y < 5)
		{
			position.y += speed * dt;
			rotateAngle -= rotateSpeed;
			rotate = true;
		}
	}

	// move player down
	if(InputManager::GetInstance()->KeyboardKeyDown(DIK_DOWN))
	{
		if(position.y > -7)
		{
			position.y -= speed * dt;
			rotateAngle += rotateSpeed;
			rotate = true;
		}
	}

	// move player left
	if(InputManager::GetInstance()->KeyboardKeyDown(DIK_LEFT))
	{
		if(position.x > -7)
			position.x -= speed * dt;
		
	}

	// move player right
	if(InputManager::GetInstance()->KeyboardKeyDown(DIK_RIGHT))
	{
		if(position.x < 8)
			position.x += speed * dt;
	}

	// check for rotation when the player moves
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

	// initiate barrel roll
	if(InputManager::GetInstance()->KeyboardKeyPressed(DIK_Z) && currentGauge == 100.0f)
	{
		barrelRoll = true;
		rotateAngle = 0.0f;	// reset object's rotation angle
	}
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
		for(DWORD j = 0; j < 
			Initializer::GetInstance()->GetPlayerMesh().numMaterials; j++)
		{
			shader->SetValue("ambientMaterial", 
				&Initializer::GetInstance()->GetPlayerMesh().modelMaterial[j].Ambient, sizeof(D3DXCOLOR));
			shader->SetValue("diffuseMaterial", 
				&Initializer::GetInstance()->GetPlayerMesh().modelMaterial[j].Diffuse, sizeof(D3DXCOLOR));
			shader->SetValue("specularMaterial", 
				&Initializer::GetInstance()->GetPlayerMesh().modelMaterial[j].Specular, sizeof(D3DXCOLOR));
			shader->SetFloat("specularPower", 
				Initializer::GetInstance()->GetPlayerMesh().modelMaterial[j].Power);
			shader->SetTexture("tex", 
				Initializer::GetInstance()->GetPlayerMesh().texture[j]);
			shader->SetBool("usingTexture", true);
			shader->CommitChanges();
			Initializer::GetInstance()->GetPlayerMesh().mesh->DrawSubset(j);
		}
		shader->EndPass();
	}
	shader->End();


	// Render the player's HUD
	playerHUD.Render(currentHealth, maxHealth, score, lives, currentGauge, maxGauge);
}

/// Release the object's variables
void Player::Shutdown()
{
}