#include "PSystem.h"

// initialize static decl
IDirect3DVertexDeclaration9* Particle::Decl  = 0;

PSystem::PSystem(const string& fxName,
	const string& techName,
	const string& texName,
	const D3DXVECTOR3 accel,
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

	// initialize values
	ID3DXBuffer* errorCheck;
	D3DXCreateEffectFromFileA(Initializer::GetInstance()->GetDevice(),
		fxName.c_str(), 0, 0, 0, 0, 
		&mFX, &errorCheck);

	if(errorCheck)
	{
		MessageBoxA(0,(char*)errorCheck->GetBufferPointer(), 0, 0);
	}

	SAFE_RELEASE(errorCheck);

	// Create Texture
	D3DXCreateTextureFromFileA(Initializer::GetInstance()->GetDevice(),
		texName.c_str(), &mTex);

	mhTech		= mFX->GetTechniqueByName(techName.c_str());
	mhWVP		= mFX->GetParameterByName(0, "gWVP");
	mhEyePosL	= mFX->GetParameterByName(0, "gEyePosL");
	mhTex		= mFX->GetParameterByName(0, "gTex");
	mhTime		= mFX->GetParameterByName(0, "gTime");
	mhAccel		= mFX->GetParameterByName(0, "gAccel");
	mhViewportHeight = mFX->GetParameterByName(0, "gViewportHeight");

	// set effect values that don't need to be updated every frame
	mFX->SetTechnique(mhTech);
	mFX->SetValue(mhAccel, mAccel, sizeof(D3DXVECTOR3));
	mFX->SetTexture(mhTex, mTex);

	Initializer::GetInstance()->GetDevice()->CreateVertexBuffer(
		mMaxNumParticles * sizeof(Particle),
		D3DUSAGE_DYNAMIC|D3DUSAGE_WRITEONLY|D3DUSAGE_POINTS,
		0, D3DPOOL_DEFAULT, &mVB, 0);

	// initialize decl
	D3DVERTEXELEMENT9 particleElements[] =
	{
		{0, 0,  D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0},
		{0, 12, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 0},
		{0, 24, D3DDECLTYPE_FLOAT1, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 1},
		{0, 28, D3DDECLTYPE_FLOAT1, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 2},
		{0, 32, D3DDECLTYPE_FLOAT1, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 3},
		{0, 36, D3DDECLTYPE_FLOAT1, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 4},
		{0, 40, D3DDECLTYPE_D3DCOLOR, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_COLOR, 0},
		D3DDECL_END()
	};

	Initializer::GetInstance()->GetDevice()->CreateVertexDeclaration(
		particleElements, &Particle::Decl);

	play = false;
}

PSystem::~PSystem(void)
{
	SAFE_RELEASE(Particle::Decl);
	SAFE_RELEASE(mFX);
	SAFE_RELEASE(mTex);
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
	mFX->SetValue(mhEyePosL, &eyePosL, sizeof(D3DXVECTOR3));
	mFX->SetFloat(mhTime, mTime);
	mFX->SetMatrix(mhWVP, &(mWorld * 
		(Camera::GetInstance()->GetViewMat() * 
		Camera::GetInstance()->GetProjMat())));

	HWND hwnd = Initializer::GetInstance()->GetD3D().hDeviceWindow;
	RECT clientRect;
	GetClientRect(hwnd, &clientRect);
	mFX->SetInt(mhViewportHeight, clientRect.bottom);

	UINT numPasses = 0;
	mFX->Begin(&numPasses, 0);
	mFX->BeginPass(0);

	Initializer::GetInstance()->GetDevice()->SetStreamSource(0, mVB, 0, 
		sizeof(Particle));
	Initializer::GetInstance()->GetDevice()->SetVertexDeclaration(Particle::Decl);

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

	mFX->EndPass();
	mFX->End();
}