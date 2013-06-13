#include "Player.h"

const float Player::speed = 10.0f;
const float Player::rotateSpeed = 0.2f;

Player::Player(void)
{
	position = D3DXVECTOR3(0, 0, 0);
	D3DXMatrixScaling(&scaleMat, 0.1f, 0.1f, 0.1f);
	rotateAngle = 0;
	currentHealth = 100.0f;
	maxHealth = 100.0f;
	score = 0;
	lives = 3;
	moveToBG = false;
	moveToFG = false;
	inBG = false;
	rollNum = 0;
	currentGauge = 100.0f;
	maxGauge = 100.0f;
	defaultBulletAttackPower = 50;
	missile1AttackPower = 100;
	status = Normal;
	ambientBlue = 0.1f;
	missile1Ammo = 2;
	starCount = 0;
	takeDamage = false;
	heal = false;
	colorCounter = 0.0f;
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

	rotate = false;

	// check the player's status
	switch(status)
	{
	case Normal:
		CheckPlayerInput(dt);
		break;
	case BarrelRoll:
		UpdateBarrelRoll(dt);
		break;
	case Dodge:
		UpdateDodge(dt);
		break;
	case MovePlane:
		MoveToSecondPlane(dt);
		break;
	}

	// check for rotation when the player moves
	if(rotate && status != BarrelRoll)
	{
		if(rotateAngle < -45.0f)
			rotateAngle = -45.0f;
		if(rotateAngle > 45.0f)
			rotateAngle = 45.0f;
	}
	else if(!rotate && status != BarrelRoll)
	{
		if(rotateAngle < 0)
			rotateAngle += rotateSpeed;
		if(rotateAngle > 0)
			rotateAngle -= rotateSpeed;
	}

	// check when the player takes damage
	if(takeDamage && colorCounter <= 1.0f)
	{
		float color = 10.0f;
		for(int i = 0; 
			i < Initializer::GetInstance()->playerMesh.numMaterials; i++)
		{
			if(Initializer::GetInstance()->playerMesh.modelMaterial[i].Ambient.r <= 0.1f)
				Initializer::GetInstance()->playerMesh.modelMaterial[i].Ambient.r = color;
			else
				Initializer::GetInstance()->playerMesh.modelMaterial[i].Ambient.r = 0.1f;
		}
		colorCounter += dt;

		if(colorCounter >= 1.0f)
		{
			takeDamage = false;
			colorCounter = 0.0f;
		}
	}
	else
	{
		for(int i = 0; 
			i < Initializer::GetInstance()->playerMesh.numMaterials; i++)
		{
			Initializer::GetInstance()->playerMesh.modelMaterial[i].Ambient.r = 0.1f;
		}
	}

	// checks when the player heals
	if(heal && colorCounter <= 1.0f)
	{
		float color = 10.0f;
		for(int i = 0; 
			i < Initializer::GetInstance()->playerMesh.numMaterials; i++)
		{
			if(Initializer::GetInstance()->playerMesh.modelMaterial[i].Ambient.b <= 0.1f)
				Initializer::GetInstance()->playerMesh.modelMaterial[i].Ambient.b = color;
			else
				Initializer::GetInstance()->playerMesh.modelMaterial[i].Ambient.b = 0.1f;
		}
		colorCounter += dt;

		if(colorCounter >= 1.0f)
		{
			heal = false;
			colorCounter = 0.0f;
		}
	}
	else
	{
		for(int i = 0; 
			i < Initializer::GetInstance()->playerMesh.numMaterials; i++)
		{
			Initializer::GetInstance()->playerMesh.modelMaterial[i].Ambient.b = 0.1f;
		}
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
}

void Player::CheckPlayerInput(float dt)
{
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
		if(!inBG)
		{
			if(position.x > -7)
				position.x -= speed * dt;
		}
		else
		{
			if(position.x < 8)
				position.x += speed * dt;
		}
	}

	// move player right
	if(InputManager::GetInstance()->KeyboardKeyDown(DIK_RIGHT))
	{
		if(!inBG)
		{
			if(position.x < 8)
				position.x += speed * dt;
		}
		else
		{
			if(position.x > -7)
				position.x -= speed * dt;
		}
	}

	// initiate barrel roll
	if(InputManager::GetInstance()->KeyboardKeyPressed(DIK_Z) && currentGauge == 100.0f)
	{
		status = BarrelRoll;
		rotateAngle = 0.0f;	// reset object's rotation angle
	}

	// initiate dodge
	else if(InputManager::GetInstance()->KeyboardKeyPressed(DIK_X) && currentGauge == 100.0f)
	{
		status = Dodge;
		moveToBG = true;
		rotateAngle = 0.0f;
	}

	// move to the second plane
	if(InputManager::GetInstance()->KeyboardKeyPressed(DIK_A))
	{
		status = MovePlane;
		rotateAngle = 0.0f;
		if(position.z == 0.0f)
		{
			moveToBG = true;
			moveToFG = false;
		}
		else
		{
			moveToBG = false;
			moveToFG = true;
		}
	}

	if(currentGauge < 100.0f)
	{
		currentGauge += 25.0f * dt;
	}
	else if(currentGauge > 100.0f)
	{
		currentGauge = 100.0f;
	}
}

void Player::UpdateBarrelRoll(float dt)
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
		status = Normal;
	}
}

void Player::UpdateDodge(float dt)
{
	currentGauge -= 150.0f * dt;

	if(currentGauge < 0.0f)
	{
		currentGauge = 0.0f;
	}

	if(moveToBG)
	{
		position.z += speed * dt;
		rotateAngle -= rotateSpeed;
		rotate = true;

		// check the player's position on the z axis
		if(position.z >= 5.0f)
		{
			moveToBG = false;
		}
	}
	else
	{
		position.z -= speed * dt;
		rotateAngle += rotateSpeed;
		rotate = true;

		if(position.z <= 0.0f)
		{
			position.z = 0.0f;
			status = Normal;
		}
	}
}

void Player::MoveToSecondPlane(float dt)
{
	// check if the player is moving to the background...
	if(moveToBG)
	{
		position.z += speed * dt;
		rotateAngle -= rotateSpeed;
		rotate = true;

		// check the player's position on the z axis
		if(position.z >= 2.5f)
		{
			D3DXVECTOR3 newEyePos(
				Camera::GetInstance()->GetEyePos().x,
				Camera::GetInstance()->GetEyePos().y,
				15.0f);
			Camera::GetInstance()->SetEyePos(newEyePos);
		}

		if(position.z >= 5.0f)
		{
			position.z = 5.0f;
			moveToBG = false;
			inBG = true;
			status = Normal;
			ambientBlue = 10.0f;
		}
	}

	// ...or if the player is moving to the foreground
	else if(moveToFG)
	{
		position.z -= speed * dt;
		rotateAngle += rotateSpeed;
		rotate = true;

		// check the player's position on the z axis
		if(position.z <= 2.5f)
		{
			D3DXVECTOR3 newEyePos(
				Camera::GetInstance()->GetEyePos().x,
				Camera::GetInstance()->GetEyePos().y,
				-10.0f);
			Camera::GetInstance()->SetEyePos(newEyePos);
		}

		if(position.z <= 0.0f)
		{
			position.z = 0.0f;
			moveToFG = false;
			inBG = false;
			status = Normal;
		}
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
	playerHUD.Render(currentHealth, maxHealth, score, lives, currentGauge, maxGauge, missile1Ammo, starCount);
}

/// Release the object's variables
void Player::Shutdown()
{
}