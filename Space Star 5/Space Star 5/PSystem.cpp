#include "PSystem.h"

// initialize static decl
//IDirect3DVertexDeclaration9* Particle::Decl  = 0;

PSystem::PSystem(const D3DXVECTOR3 accel,
	const AABB& box,
	int maxNumParticles,
	float timePerParticle)
{
	mBox = box;
	mAccel = accel;
	mTime = 0.0f;
	mMaxNumParticles = maxNumParticles;
	mTimePerParticle = timePerParticle;

	// allocate memory for maximum number of particles
	mParticles.resize(mMaxNumParticles);
	mAliveParticles.reserve(mMaxNumParticles);
	mDeadParticles.reserve(mMaxNumParticles);

	// particles start off dead
	for(int i = 0; i < mMaxNumParticles; ++i)
	{
		mParticles[i].lifeTime = -1.0f;
		mParticles[i].initTime = 0.0f;
	}

	D3DXMatrixIdentity(&mWorld);
	D3DXMatrixIdentity(&mInvWorld);

 	mhTech		= Initializer::GetInstance()->GetPSysFX()->GetTechniqueByName("FireRingTech");
	mhWVP		= Initializer::GetInstance()->GetPSysFX()->GetParameterByName(0, "gWVP");
	mhEyePosL	= Initializer::GetInstance()->GetPSysFX()->GetParameterByName(0, "gEyePosL");
	mhTex		= Initializer::GetInstance()->GetPSysFX()->GetParameterByName(0, "gTex");
	mhTime		= Initializer::GetInstance()->GetPSysFX()->GetParameterByName(0, "gTime");
	mhAccel		= Initializer::GetInstance()->GetPSysFX()->GetParameterByName(0, "gAccel");
	mhViewportHeight = Initializer::GetInstance()->GetPSysFX()->GetParameterByName(0, "gViewportHeight");

	// set effect values that don't need to be updated every frame
	Initializer::GetInstance()->GetPSysFX()->SetTechnique(mhTech);
	Initializer::GetInstance()->GetPSysFX()->SetValue(mhAccel, mAccel, sizeof(D3DXVECTOR3));
	Initializer::GetInstance()->GetPSysFX()->SetTexture(mhTex, Initializer::GetInstance()->GetPSysTex());

	Initializer::GetInstance()->GetDevice()->CreateVertexBuffer(
		mMaxNumParticles * sizeof(Particle),
		D3DUSAGE_DYNAMIC|D3DUSAGE_WRITEONLY|D3DUSAGE_POINTS,
		0, D3DPOOL_DEFAULT, &mVB, 0);

	play = false;
	destroyObject = false;
}

PSystem::~PSystem(void)
{
	SAFE_RELEASE(mVB);
}

void PSystem::SetWorldMat(const D3DXMATRIX& world)
{
	mWorld = world;
	D3DXMatrixInverse(&mInvWorld, 0, &mWorld);
}

void PSystem::AddParticle()
{
	// if there are no dead particles, we are maxed out
	if(mDeadParticles.size() > 0)
	{
		// reinitialize a particle
		Particle* p = mDeadParticles.back();
		InitParticle(*p);

		// no longer dead
		mDeadParticles.pop_back();
		mAliveParticles.push_back(p);
	}
}

void PSystem::Update(float dt)
{
	mTime += dt;

	// Rebuild the dead and alive list
	mDeadParticles.resize(0);
	mAliveParticles.resize(0);

	// for each particle
	for(int i = 0; i < mMaxNumParticles; ++i)
	{
		// is particle dead?
		if((mTime - mParticles[i].initTime) > mParticles[i].lifeTime)
		{
			mDeadParticles.push_back(&mParticles[i]);
		}
		else
		{
			mAliveParticles.push_back(&mParticles[i]);
		}
	}

	// a negative or zero mTimePerParticle value denotes not to emit
	// any particles
	if(mTimePerParticle > 0.0f)
	{
		// emit particles
		static float timeAccum = 0.0f;
		timeAccum += dt;

		while(timeAccum >= mTimePerParticle)
		{
			AddParticle();
			timeAccum -= mTimePerParticle;
		}
	}
}

void PSystem::Render()
{
	D3DXVECTOR3 eyePosW = Camera::GetInstance()->GetEyePos();
	D3DXVECTOR3 eyePosL;
	D3DXVec3TransformCoord(&eyePosL, &eyePosW, &mInvWorld);

	// set effect parameters
	Initializer::GetInstance()->GetPSysFX()->SetValue(mhEyePosL, &eyePosL, sizeof(D3DXVECTOR3));
	Initializer::GetInstance()->GetPSysFX()->SetFloat(mhTime, mTime);
	Initializer::GetInstance()->GetPSysFX()->SetMatrix(mhWVP, &(mWorld * 
		(Camera::GetInstance()->GetViewMat() * 
		Camera::GetInstance()->GetProjMat())));

	HWND hwnd = Initializer::GetInstance()->GetD3D().hDeviceWindow;
	RECT clientRect;
	GetClientRect(hwnd, &clientRect);
	Initializer::GetInstance()->GetPSysFX()->SetInt(mhViewportHeight, clientRect.bottom);

	UINT numPasses = 0;
	Initializer::GetInstance()->GetPSysFX()->Begin(&numPasses, 0);
	Initializer::GetInstance()->GetPSysFX()->BeginPass(0);

	Initializer::GetInstance()->GetDevice()->SetStreamSource(0, mVB, 0, 
		sizeof(Particle));
	Initializer::GetInstance()->GetDevice()->SetVertexDeclaration(
		Initializer::GetInstance()->GetDecl());

	AABB boxWorld;
	mBox.xform(mWorld, boxWorld);
	if(Camera::GetInstance()->IsVisible(boxWorld))
	{
		// initial lock of VB for writing
		Particle* p = 0;
		mVB->Lock(0, 0, (void**)&p, D3DLOCK_DISCARD);
		int vbIndex = 0;

		// for each living particle
		for(UINT i = 0; i < mAliveParticles.size(); ++i)
		{
			// copy particle to VB
			p[vbIndex] = *mAliveParticles[i];
			++vbIndex;
		}
		mVB->Unlock();

		// render however many particles we copied over
		if(vbIndex > 0)
		{
			Initializer::GetInstance()->GetDevice()->DrawPrimitive(
				D3DPT_POINTLIST, 0, vbIndex);
		}
	}

	Initializer::GetInstance()->GetPSysFX()->EndPass();
	Initializer::GetInstance()->GetPSysFX()->End();
}