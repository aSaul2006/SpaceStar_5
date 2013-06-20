#pragma once
#include "AABB.h"
#include "Camera.h"
#include "Initializer.h"
#include <string>
#include <vector>
#include <time.h>

using namespace std;

/// Particle Structure
struct Particle
{
	D3DXVECTOR3 initPos;	// initial position
	D3DXVECTOR3 initVel;	// initial velocity
	float		initSize;	// initial size
	float		initTime;	// initial time
	float		lifeTime;	// life time of the particle
	float		mass;		// mass of the particle
	D3DCOLOR	initColor;	// initial color
};

class PSystem
{
protected:
	D3DXHANDLE mhTech;
	D3DXHANDLE mhWVP;
	D3DXHANDLE mhEyePosL;
	D3DXHANDLE mhTex;
	D3DXHANDLE mhTime;
	D3DXHANDLE mhAccel;
	D3DXHANDLE mhViewportHeight;

	
	IDirect3DVertexBuffer9* mVB;
	D3DXMATRIX mWorld;
	D3DXMATRIX mInvWorld;
	float mTime;
	D3DXVECTOR3 mAccel;
	AABB mBox;
	int mMaxNumParticles;
	float mTimePerParticle;
	bool destroyObject;

	vector<Particle> mParticles;
	vector<Particle*> mAliveParticles;
	vector<Particle*> mDeadParticles;
public:
	PSystem(
		const D3DXVECTOR3 accel,
		const AABB& box,
		int maxNumParticles,
		float timePerParticle);

	virtual ~PSystem(void);

	// Access methods
	float GetTime() {return mTime;}
	void SetTime(float t) {mTime = t;}
	const AABB& GetAABB()const {return mBox;}

	void SetWorldMat(const D3DXMATRIX& world);
	void AddParticle();

	virtual void InitParticle(Particle& out) = 0;
	virtual void Update(float dt);
	virtual void Render();

	bool play;
	void ResetTime(void){mTime = 0;}

	bool CheckObject(void) {return destroyObject;}
	void Destroy(void) {destroyObject = true;}
};

class FireRing : public PSystem
{
public:
	FireRing(
		const D3DXVECTOR3 accel,
		const AABB& box,
		int maxNumParticles,
		float timePerParticle)
		: PSystem(accel, box, 
		maxNumParticles, timePerParticle)
	{
	}

	// generates a random float
	float GetRandomFloat(float a, float b)
	{
		if( a >= b) // bad input
			return a;

		// Get random float in [0, 1] interval
		float f = (rand() % 10001) * 0.0001f;

		return (f * (b - a)) + a;
	}

	// generates a random vector
	void GetRandomVec(D3DXVECTOR3& out)
	{
		out.x = GetRandomFloat(-1.0f, 1.0f);
		out.y = GetRandomFloat(-1.0f, 1.0f);
		out.z = GetRandomFloat(-1.0f, 1.0f);

		D3DXVec3Normalize(&out, &out);
	}

	void InitParticle(Particle& out)
	{
		// time particle is created relative to the global running
		// time of the particle system
		out.initTime = mTime;

		// Flare lives for 2-4 seconds
		out.lifeTime = GetRandomFloat(2.0f, 4.0f);

		// initial size of pixels
		out.initSize = GetRandomFloat(0.0f, 0.1f);

		// give a very small initial velocity to give the flares
		// some randomness
		GetRandomVec(out.initVel);

		// scalar value used in vertex shader as an
		// amplitude factor
		out.mass = GetRandomFloat(0.0f, 0.1f);

		// Start color at 50-100% intensity when born for
		// variation
		D3DCOLOR WHITE = D3DCOLOR_RGBA(255, 255, 255, 255);
		out.initColor = (D3DCOLOR)GetRandomFloat(0.5f, 1.0f) * WHITE;

		// generate random particle on the ring in polar
		// coordinates: random radius and random angle
		float r = GetRandomFloat(0.0f, 0.1f);
		float t = GetRandomFloat(0, 2.0f * D3DX_PI);

		// convert to cartesian coordinates
		out.initPos.x = r * cosf(t);
		out.initPos.y = r * sinf(t);

		// random depth value in [-1, 1] (depth of ring)
		out.initPos.z = GetRandomFloat(-1.0f, 1.0f);
	}
};