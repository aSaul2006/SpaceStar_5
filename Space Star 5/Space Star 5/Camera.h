#pragma once

#include <d3d9.h>
#include <d3dx9.h>
#include "InputManager.h"
#include "AABB.h"

#pragma comment(lib, "d3d9.lib")
#pragma comment(lib, "d3dx9.lib")

class Camera
{
private:
	static Camera* instance;
	Camera(void);

	D3DXVECTOR3 eyePos, lookAt, upVec;
	D3DXMATRIX projectionMatrix, viewMatrix;
	D3DXPLANE frustumPlanes[6];

	float cameraSpeed;
public:
	~Camera(void);

	static Camera* GetInstance(void);
	static void DestroyInstance(void);

	void Initialize(int screenWidth, int screenHeight);
	void Update(float dt);
	void Movement(float dt);
	void BuildWorldFrustumPlanes();
	bool IsVisible(const AABB& box) const;

	// Assessors
	D3DXMATRIX GetProjMat() {return projectionMatrix;}
	D3DXMATRIX GetViewMat() {return viewMatrix;}
	D3DXVECTOR3 GetEyePos() {return eyePos;}
	void IncreaseEyePos(int num);
	void DecreaseEyePos(int num);
	
	float GetCameraSpeed() {return cameraSpeed;}

	void SetEyePos(D3DXVECTOR3 newPos) {eyePos = newPos;}
};