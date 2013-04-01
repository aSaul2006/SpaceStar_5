#include "Camera.h"

Camera* Camera::instance = 0;

Camera::Camera(void)
{
}

Camera::~Camera(void)
{
	DestroyInstance();
}

Camera* Camera::GetInstance(void)
{
	if(instance == 0)
		instance = new Camera();
	return instance;
}
	
void Camera::DestroyInstance(void)
{
	if(instance)
	{
		delete instance;
		instance = NULL;
	}
}

void Camera::Initialize(int screenWidth, int screenHeight)
{
	///////////////////////////////////////////////////////////////////
	// Camera Variables
	///////////////////////////////////////////////////////////////////

	// Initial camera position
	eyePos = D3DXVECTOR3 (0, 2, -10);
	lookAt = D3DXVECTOR3 (0, 0, 0);
	upVec = D3DXVECTOR3(0, 1, 0);

	// Set the projection matrix
	D3DXMatrixPerspectiveFovLH(&projectionMatrix, D3DXToRadian(65.0f), 
		(float)screenWidth / (float)screenHeight, 1.0f, 1000.0f);
}

void Camera::Update(float dt)
{
	Movement(dt);
	BuildWorldFrustumPlanes();
}

void Camera::Movement(float dt)
{
	// camera update
	float cameraSpeed = 10.0f;

	if(InputManager::GetInstance()->KeyboardKeyDown(DIK_W))
	{
		eyePos.y += cameraSpeed * dt;
		lookAt.y += cameraSpeed * dt;
	}

	if(InputManager::GetInstance()->KeyboardKeyDown(DIK_S))
	{
		eyePos.y -= cameraSpeed * dt;
		lookAt.y -= cameraSpeed * dt;
	}

	if(InputManager::GetInstance()->KeyboardKeyDown(DIK_A))
	{
		eyePos.x -= cameraSpeed * dt;
		lookAt.x -= cameraSpeed * dt;
	}

	if(InputManager::GetInstance()->KeyboardKeyDown(DIK_D))
	{
		eyePos.x += cameraSpeed * dt;
		lookAt.x += cameraSpeed * dt;
	}

	// Set view matrix
	D3DXMatrixLookAtLH(&viewMatrix, &eyePos, &lookAt, &upVec);
}

void Camera::BuildWorldFrustumPlanes()
{
	// Note: Extract the frustum planes in world space.

	D3DXMATRIX VP = viewMatrix * projectionMatrix;

	D3DXVECTOR4 col0(VP(0,0), VP(1,0), VP(2,0), VP(3,0));
	D3DXVECTOR4 col1(VP(0,1), VP(1,1), VP(2,1), VP(3,1));
	D3DXVECTOR4 col2(VP(0,2), VP(1,2), VP(2,2), VP(3,2));
	D3DXVECTOR4 col3(VP(0,3), VP(1,3), VP(2,3), VP(3,3));

	// Planes face inward.
	frustumPlanes[0] = (D3DXPLANE)(col2);        // near
	frustumPlanes[1] = (D3DXPLANE)(col3 - col2); // far
	frustumPlanes[2] = (D3DXPLANE)(col3 + col0); // left
	frustumPlanes[3] = (D3DXPLANE)(col3 - col0); // right
	frustumPlanes[4] = (D3DXPLANE)(col3 - col1); // top
	frustumPlanes[5] = (D3DXPLANE)(col3 + col1); // bottom

	for(int i = 0; i < 6; i++)
		D3DXPlaneNormalize(&frustumPlanes[i], &frustumPlanes[i]);
}

bool Camera::IsVisible(const AABB& box) const
{
	// Test assumes frustum planes face inward.

	D3DXVECTOR3 P;
	D3DXVECTOR3 Q;

	//      N  *Q                    *P
	//      | /                     /
	//      |/                     /
	// -----/----- Plane     -----/----- Plane    
	//     /                     / |
	//    /                     /  |
	//   *P                    *Q  N
	//
	// PQ forms diagonal most closely aligned with plane normal.

	// For each frustum plane, find the box diagonal (there are four main
	// diagonals that intersect the box center point) that points in the
	// same direction as the normal along each axis (i.e., the diagonal 
	// that is most aligned with the plane normal).  Then test if the box
	// is in front of the plane or not.
	for(int i = 0; i < 6; ++i)
	{
		// For each coordinate axis x, y, z...
		for(int j = 0; j < 3; ++j)
		{
			// Make PQ point in the same direction as the plane normal on this axis.
			if( frustumPlanes[i][j] >= 0.0f )
			{
				P[j] = box.minPt[j];
				Q[j] = box.maxPt[j];
			}
			else 
			{
				P[j] = box.maxPt[j];
				Q[j] = box.minPt[j];
			}
		}

		// If box is in negative half space, it is behind the plane, and thus, completely
		// outside the frustum.  Note that because PQ points roughly in the direction of the 
		// plane normal, we can deduce that if Q is outside then P is also outside--thus we
		// only need to test Q.
		if( D3DXPlaneDotCoord(&frustumPlanes[i], &Q) < 0.0f  ) // outside
			return false;
	}
	return true;
}