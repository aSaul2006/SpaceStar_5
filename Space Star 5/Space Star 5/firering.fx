// Combined World-View-Projection matrix
uniform extern float4x4 gWVP;

// Particle texture
uniform extern texture gTex;

// Position of the camera in the particle system's
// local space for distance calculations
uniform extern float3 gEyePosL;

// Constant acceleration vector
uniform extern float3 gAccel;

// Particle system time -- corresponds to PSystem::mTime
uniform extern float gTime;

// Viewport height for scaling the point sprite sizes
// see comment in PSystem::Render
uniform extern int gViewportHeight;

sampler TexS = sampler_state
{
	Texture = <gTex>;
	MinFilter = LINEAR;
	MagFilter = LINEAR;
	MipFilter = POINT;
	AddressU = CLAMP;
	AddressV = CLAMP;
};

struct OutputVS
{
	float4 posH : POSITION0;
	float4 color : COLOR0;
	float2 tex0 : TEXCOORD0;
	float size : PSIZE;
};

// Input corresponds to the Particle structure data members
OutputVS FireRingVS(float3 posL		: POSITION0,
					float3 vel		: TEXCOORD0,
					float size		: TEXCOORD1,
					float time		: TEXCOORD2,
					float lifeTime 	: TEXCOORD3,
					float mass		: TEXCOORD4,
					float4 color	: COLOR0)
{
	// Zero out our output
	OutputVS outVS = (OutputVS)0;
	
	// Get age of particle from creation time
	float t = gTime - time;
	
	// Rotate the particles about local space about z-axis as a
	// function of time. These are just the rotation equations
	float sine, cosine;
	sincos(0.5 * mass * t, sine, cosine);
	float x = posL.x * cosine + posL.y * -sine;
	float y = posL.x * sine + posL.y * cosine;
	
	// Oscillate particles up and down
	float s = sin(6.0f * t);
	posL.x = x;
	posL.y = y + mass * s;
	
	// Constant acceleration
	posL = posL + vel * t + 0.5f * gAccel * t * t;
	
	// Transform to homogeneous clip space
	outVS.posH = mul(float4(posL, 1.0f), gWVP);
	
	// Ramp up size over time to simulate the flare expanding
	// over time. Formula found by experimenting
	size += 8.0f * t * t;
	
	// Also compute size as a function of the distance from
	// the camera, and the viewport height. The constants
	// were found by experimenting
	float d = distance(posL, gEyePosL);
	outVS.size = gViewportHeight * size / (1.0f + 8.0f * d);
	
	// Fade color from white to black over particle's lifetime
	// to fade it out gradually
	outVS.color = (1.0f - (t / lifeTime));
	
	// Done -- return the output
	return outVS;
}

float4 FireRingPS(float4 color : COLOR0,
					float2 tex0 : TEXCOORD0) : COLOR
{
	// Multiply the faded color with the texture color
	return color * tex2D(TexS, tex0);
}

technique FireRingTech
{
	pass P0
	{
		vertexShader = compile vs_2_0 FireRingVS();
		pixelShader = compile ps_2_0 FireRingPS();
		
		PointSpriteEnable = true;
		AlphaBlendEnable = true;
		SrcBlend = One;
		DestBlend = One;
		
		ZWriteEnable = false;
	}
}