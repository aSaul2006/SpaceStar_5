#include "TutorialScreen.h"

const short TutorialScreen::maxChoice = 6;

TutorialScreen::TutorialScreen(void)
{
	type = TutorialType;
	pause = true;
	pauseTime = 0.0f;
	choice = 1;

	Initialize();
}


TutorialScreen::~TutorialScreen(void)
{
	Shutdown();
}

void TutorialScreen::Initialize()
{
	// initialize player
	player.Initialize();
	player.SetTutorial(true);
	player.SetPosition(D3DXVECTOR3(0.0f, 0.0f, 0.0f));

	// Create Skybox
	float screenWidth = 800;
	float screenHeight = 600;
	skybox.BuildSkybox((float)screenWidth, (float)screenHeight);

	// create shader effect
	D3DXCreateEffectFromFile(Initializer::GetInstance()->GetDevice(),
		L"lab5.fx", 0, 0, 0, 0, 
		&shader, &errorCheck);
	if(errorCheck)
	{
		MessageBoxA(0,(char*)errorCheck->GetBufferPointer(), 0, 0);
	}
	SAFE_RELEASE(errorCheck);

	// Initialize SFX
	AudioManager::GetInstance()->GetSystem()->createSound("8bitLaser1.wav", 
		FMOD_DEFAULT, 0, &projSFX);
}

void TutorialScreen::Update(GameState& gameState, float dt)
{
	// update camera instance
	Camera::GetInstance()->Update(dt);

	// update player
	player.Update(dt);

	// update skybox
	skybox.Update(dt);

	switch(choice)
	{
	case 1:
		PageUpdate1(dt);
		break;
	case 2:
		PageUpdate2(dt);
		break;
	case 3:
		PageUpdate3(dt);
		break;
	case 4:
		PageUpdate4(dt);
		break;
	case 5:
		PageUpdate5(dt);
		break;
	case 6:
		PageUpdate6(dt);
		break;
	}

	if(pause)
	{
		pauseTime += dt;
		if(pauseTime >= 1.0f)
		{
			pause = false;
			pauseTime = 0.0f;
		}
	}

	if(InputManager::GetInstance()->KeyboardKeyPressed(DIK_ESCAPE))
	{
		gameState = TitleMenu;
	}

	if(InputManager::GetInstance()->KeyboardKeyPressed(DIK_LEFT))
	{
		choice--;

		if(choice < 1)
			choice = 1;

		player.ResetPlayer();
		pause = true;
		pauseTime = 0.0f;
	}

	if(InputManager::GetInstance()->KeyboardKeyPressed(DIK_RIGHT))
	{
		choice++;

		if(choice > maxChoice)
			choice = maxChoice;

		player.ResetPlayer();
		pause = true;
		pauseTime = 0.0f;
	}

	// delete projectiles from list if it's not the correct tutorial page
	if(choice != 3)
	{
		for each(Projectile* projectile in pList)
		{
			projectile->Destroy();
		}
	}

	// check for projectiles that need to be removed from the list
	// and remove them
	for(list<Projectile*>::const_iterator i = pList.begin(), end = pList.end(); i != end;)
	{	
		if((*i)->CheckObject())
		{
			delete (*i);
			i = pList.erase(i);
		}
		else
			i++;		
	}
}

// Shows the player how to move up and down
void TutorialScreen::PageUpdate1(float dt)
{
	if(player.GetPosition().y == 0.0f)
	{
		direction = D3DXVECTOR3(0.0f, 10.0f, 0.0f);
	}
	else if(player.GetPosition().y == 5.0f)
	{
		direction = D3DXVECTOR3(0.0f, -10.0f, 0.0f);
	}

	if(!pause)
	{
		player.ModifyPosition(direction, dt);

		if(player.GetPosition().y == 0.0f ||
			player.GetPosition().y == 5.0f)
			pause = true;
	}

	print = "Press \"Up\" arrow key to move up\n"
		"Press \"Down\" arrow key to move down";
}

// Shows the player how to move left and right
void TutorialScreen::PageUpdate2(float dt)
{
	if(player.GetPosition().x == 0.0f)
	{
		direction = D3DXVECTOR3(-10.0f, 0.0f, 0.0f);
	}
	else if(player.GetPosition().x == -7.0f)
	{
		direction = D3DXVECTOR3(10.0f, 0.0f, 0.0f);
	}

	if(!pause)
	{
		player.ModifyPosition(direction, dt);

		if(player.GetPosition().x == 0.0f ||
			player.GetPosition().x == -7.0f)
			pause = true;
	}
	print = "Press \"Right\" arrow key to move right\n"
		"Press \"Left\" arrow key to move left";
}

void TutorialScreen::PageUpdate3(float dt)
{
	if(!pause)
	{
		pList.push_front(new DefaultBullet(player.GetPosition(), D3DXVECTOR3(20.0f, 0, 0)));
		AudioManager::GetInstance()->PlaySFX(projSFX);
		pause = true;
	}

	// update the projectiles in pList
	for each(Projectile* projectile in pList)
	{
		// update projectile
		projectile->Update(dt);
	}

	print = "Press \"Space\" key to fire a bullet";
}

void TutorialScreen::PageUpdate4(float dt)
{
	if(!pause)
	{
		player.DoBarrelRoll();
		pause = true;
	}

	print = "Press \"Z\" key to do a barrel roll";
}

void TutorialScreen::PageUpdate5(float dt)
{
	if(!pause)
	{
		player.DoDodge();
		pause = true;
	}

	print = "Press \"X\" key to dodge";
}

void TutorialScreen::PageUpdate6(float dt)
{
	if(!pause)
	{
		player.DoPlaneChange();
		pause = true;
	}

	print = "Press \"A\" key to move to the\nsecond plane";
}

void TutorialScreen::Render(void)
{
	// Set Z render state to false to ignore Z-coordinates when
	// rendering the sky box
	Initializer::GetInstance()->GetDevice()->SetRenderState(D3DRS_ZWRITEENABLE, false);

	// Render the sky box
	skybox.Render();

	// Set Z render state to true when finishing rendering 
	// the sky box
	Initializer::GetInstance()->GetDevice()->SetRenderState(D3DRS_ZWRITEENABLE, true);

	// Render player ship
	player.Render(shader);

	// render projectiles
	for each(Projectile* projectile in pList)
	{

		projectile->Render(shader);
		
	}


	RECT printPos;

	SetRect(&printPos, 0, 400, 800, 500);
	Initializer::GetInstance()->GetFont()->DrawTextA(0, print.c_str(),
		-1, &printPos, 
		DT_CENTER | DT_NOCLIP,
		D3DCOLOR_RGBA(255, 255, 255, 255));

	string menu = "Left: Prev";
	SetRect(&printPos, 0, 500, 800, 600);
	Initializer::GetInstance()->GetFont()->DrawTextA(0, menu.c_str(),
		-1, &printPos, 
		DT_LEFT | DT_NOCLIP,
		D3DCOLOR_RGBA(255, 255, 255, 255));

	menu = to_string(static_cast<long long>(choice)) + "/" + 
		to_string(static_cast<long long>(maxChoice));
	Initializer::GetInstance()->GetFont()->DrawTextA(0, menu.c_str(),
		-1, &printPos, 
		DT_CENTER | DT_NOCLIP,
		D3DCOLOR_RGBA(255, 255, 255, 255));

	menu = "Right: Next";
	Initializer::GetInstance()->GetFont()->DrawTextA(0, menu.c_str(),
		-1, &printPos, 
		DT_RIGHT | DT_NOCLIP,
		D3DCOLOR_RGBA(255, 255, 255, 255));
}

void TutorialScreen::Shutdown(void)
{
	projSFX->release();
	SAFE_RELEASE(shader);
	SAFE_RELEASE(errorCheck);

	if(pList.size() > 0)
	{
		for(list<Projectile*>::const_iterator i = pList.begin(), 
			end = pList.end(); i != end; )
		{
			delete (*i);
			i = pList.erase(i);
		}
	}
}