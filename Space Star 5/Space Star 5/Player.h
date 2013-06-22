#pragma once

#include "InputManager.h"
#include "Camera.h"
#include "HUD.h"

// safe release macro
#define SAFE_RELEASE(x) if(x){x->Release(); x = 0;}

enum PlayerStatus
	{
		Normal,
		BarrelRoll,
		Dodge,
		MovePlane,
	};

class Player
{
private:
	D3DXMATRIX scaleMat, rotateMat, translateMat, worldMat;
	D3DXVECTOR3 position;

	float rotateAngle;
	AABB meshBox;	// mesh's collision box

	// player HUD variable
	HUD playerHUD;

	// player health variables
	float currentHealth;
	float maxHealth;

	// player score variable
	int score;

	//player attack power
	int defaultBulletAttackPower;
	int missile1AttackPower;
	
	// player lives variable
	short lives;

	// player missile1 ammo variable
	short missile1Ammo;

	// accrued star's
	int starCount;

	// indicates whether the player is performing a barrel roll
	short rollNum;
	float currentGauge;
	float maxGauge;

	// indicates when the player uses the dodge command
	bool moveToBG;	// move to the background
	bool moveToFG;	// move to the foreground
	bool inBG;		// checks to see if the player is in the background or not

	bool rotate;
	PlayerStatus status;
	float ambientBlue;

	// const variables
	static const float speed;
	static const float rotateSpeed;

	// variables indicate whether the player takes damage or
	// player heals
	bool takeDamage;
	bool heal;
	float colorCounter;	// used to change color for a period of time

	bool isTutorial;	// puts the player in its tutorial state

public:
	Player(void);
	~Player(void);

	void Initialize();
	void Update(float dt);
	void Render(ID3DXEffect* shader);
	void Shutdown();

	// Accessors
	D3DXVECTOR3 GetPosition() {return position;} // get position
	
	// get collision box
	AABB GetMeshBox()
	{
		AABB out;
		meshBox.xform(worldMat, out);
		return out;
	}

	// Dummy ship functions for collision testing
	// Delete after testing collision
	void SetPosition(D3DXVECTOR3 pos) {position = pos;}

	// Modify player's current health
	void IncrCurrHlth(float incrNum) {currentHealth += incrNum; heal = true;}
	void DecrCurrHlth(float decrNum) {currentHealth -= decrNum; takeDamage = true;}
	void SetCurrHlth(float setHlth)	{currentHealth = setHlth;}

	// Modify player's max health
	void IncrMaxHlth(float incrNum) {maxHealth += incrNum;}
	void DecrMaxHlth(float decrNum) {maxHealth -= decrNum;}
	void SetMaxHlth(float setHlth) {maxHealth = setHlth;}

	// Access player's health
	float GetCurrHlth(void) {return currentHealth;}
	float GetMaxHlth(void)	{return maxHealth;}

	short GetNumLives(void) {return lives;}
	void IncrLives(void) {lives = lives + 1;}

	// Modify and access player's score
	void IncrScore(int score) {this->score += score;}
	int GetScore(void) {return score;}

	//modify and access players attack power
	void AugmentDefaultBulletPower(int power){this->defaultBulletAttackPower = power;}
	int GetDefaultBulletPower(){return this->defaultBulletAttackPower;}
	void AugmentMissile1AttackPower(int power) {this->missile1AttackPower = power;}
	int GetMissile1AttackPower(){return this->missile1AttackPower;}

	//modify and access players missile1 amount
	void setMissile1Amount(int value) {missile1Ammo = value;}
	void IncrMissile1Amount(int value){missile1Ammo = missile1Ammo + value;}
	void DecrMissile1Amount(int value)
	{
		if(missile1Ammo > 0)
			missile1Ammo = missile1Ammo - value;
	}
	short GetMissile1Amount(){return missile1Ammo;}

	//modify and access players star count
	void IncrStarCount() {starCount = starCount + 1;}
	void SetStarCountToZero() {starCount = 0;}
	int GetStarCount() {return starCount;}

	// Player behaviors
	void CheckPlayerInput(float dt);
	void UpdateBarrelRoll(float dt);
	void UpdateDodge(float dt);
	void MoveToSecondPlane(float dt);
	PlayerStatus GetStatus(void) {return status;}

	// set player's tutorial state
	void SetTutorial(bool tutorial) {isTutorial = tutorial;}

	/////////////////////////////////////////////////////////////////////////////////////////////////////////
	// tutorial functions
	/////////////////////////////////////////////////////////////////////////////////////////////////////////

	// Note: only use these functions in the tutorial screen
	void ModifyPosition(D3DXVECTOR3 newPos, float dt) 
	{
		position += newPos * dt;

		if(newPos.y != 0.0f)
		{
			if(newPos.y > 0.0f)
			{
				rotateAngle -= rotateSpeed;
				rotate = true;
			}
			if(newPos.y < 0.0f)
			{
				rotateAngle += rotateSpeed;
				rotate = true;
			}
		}

		// clip player
		if(position.y >= 5.0f)
			position.y = 5.0f;
		if(position.y <= 0.0f)
			position.y = 0.0f;
		if(position.x >= 0.0f)
			position.x = 0.0f;
		if(position.x <= -7.0f)
			position.x = -7.0f;
	}

	// Note: only use these functions in the tutorial screen
	void DoBarrelRoll(void)
	{
		if(currentGauge == 100.0f)
		{
			status = BarrelRoll;
			rotateAngle = 0.0f;	// reset object's rotation angle
		}
	}

	// Note: only use these functions in the tutorial screen
	void DoDodge(void)
	{
		if(currentGauge == 100.0f)
		{
			status = Dodge;
			moveToBG = true;
			rotateAngle = 0.0f;
		}
	}

	// Note: only use these functions in the tutorial screen
	void DoPlaneChange(void)
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

	// Note: only use these functions in the tutorial screen
	void ResetPlayer(void)
	{
		position = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		rotateAngle = 0.0f;
		rotate = false;
		moveToBG = false;
		moveToFG = false;
		status = Normal;
		Camera::GetInstance()->SetEyePos(D3DXVECTOR3(
			Camera::GetInstance()->GetEyePos().x,
				Camera::GetInstance()->GetEyePos().y,
				-10.0f));
		currentGauge = 100.0f;
	}
};