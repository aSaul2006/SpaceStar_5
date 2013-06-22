#pragma once
#include "screen.h"
#include "Player.h"
#include "Skybox.h"
#include "AudioManager.h"
#include "Projectile.h"
class TutorialScreen :
	public CScreen
{
private:
	Player player;
	ID3DXEffect* shader;
	ID3DXBuffer* errorCheck;
	Skybox skybox;

	short choice;
	static const short maxChoice;

	bool pause;
	float pauseTime;

	D3DXVECTOR3 direction;

	FMOD::Sound* projSFX;	// SFX for the player's projectile

	list<Projectile*> pList;
public:
	TutorialScreen(void);
	~TutorialScreen(void);

	void Initialize(void);
	void Update(GameState& gameState, float dt);
	void Render(void);
	void Shutdown(void);

	// update and render functions for each page
	void PageUpdate1(float dt);
	void PageUpdate2(float dt);
	void PageUpdate3(float dt);
	void PageUpdate4(float dt);
	void PageUpdate5(float dt);
	void PageUpdate6(float dt);
};

