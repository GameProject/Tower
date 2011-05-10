#include "Camera.h"

Camera& GetCamera()
{
	static Camera camera;
	return camera;
}

Camera::Camera()
{
	mPosition = D3DXVECTOR3(0.0f, 20.0f, 0.0f);
	mRight    = D3DXVECTOR3(1.0f, 0.0f, 0.0f);
	mUp       = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	mLook     = D3DXVECTOR3(0.0f, -1.0f, 0.0f);

	D3DXMatrixIdentity(&mView);
	D3DXMatrixIdentity(&mProj);

	crouching = false;
	offsetY = 2.5f;
	jumpDown = false;
	jumping = false;
}

D3DXVECTOR3 Camera::getPos()
{
	return mPosition;
}

D3DXVECTOR3 Camera::getLook()
{
	return mLook;
}

D3DXMATRIX Camera::view()const
{
	return mView;
}

D3DXMATRIX Camera::proj()const
{
	return mProj;
}

D3DXMATRIX Camera::wvp(D3DXMATRIX world)
{
	return world*mView*mProj;
}

void Camera::setPosY(float y)
{
	
	if (jumping)
	{
		if (!jumpDown && y+2.5f < jumpMIN)
		{
			offsetY += y;
			stopJump();
		}
		mPosition.y = y+offsetY;
		jumpMIN = y+2.5f;
	}
	else
		mPosition.y = y+offsetY;
}

void Camera::setPos(D3DXVECTOR3 pos)
{
	mPosition = pos;
}

void Camera::setLens(float fovY, float aspect, float zn, float zf)
{
	float h = cosf(fovY/2)/sinf(fovY/2);
	float w = h / aspect;

	mProj._11 = w;
	mProj._21 = 0.0f;
	mProj._31 = 0.0f;
	mProj._41 = 0.0f;

	mProj._12 = 0.0f;
	mProj._22 = h;
	mProj._32 = 0.0f;
	mProj._42 = 0.0f;

	mProj._13 = 0.0f;
	mProj._23 = 0.0f;
	mProj._33 = zf/(zf-zn);
	mProj._43 = zn*zf/(zn-zf);

	mProj._14 = 0.0f;
	mProj._24 = 0.0f;
	mProj._34 =	1.0f;
	mProj._44 = 0.0f;
}

void Camera::strafe(float dt)
{
	if (!jumping)
		mPosition += dt*mRight;
	else
		mPosition += 2*dt*mRight;
	mLastDeltaTime = dt;
}

void Camera::walk(float dt)
{
	if (!jumping)
		mPosition += dt*mLook;
	else
		mPosition += 2*dt*mLook;
	mLastDeltaTime = dt;
}

void Camera::crouch()
{
	if (!jumping)
	{
		if (!crouching)
		{
			offsetY = 1.5f;
			crouching = true;
		}
		else
		{
			offsetY = 2.5f;
			crouching = false;
		}
	}
}

void Camera::setJump(float t)
{
	jumpMIN = mPosition.y;
	if (!crouching)
		jumpMAX = mPosition.y+3.0f;
	else
		jumpMAX = mPosition.y+1.5f;
	jumping = true;
	start = t;
}

bool Camera::jump(float t)
{
	float c = t - start;
	if (mPosition.y < jumpMAX && !jumpDown)
		offsetY += c;
	else
	{
		jumpDown = true;
		if (mPosition.y > jumpMIN)
			offsetY -= c;
		else
		{	
			stopJump();
			return false;
		}
	}
	return true;
}

void Camera::stopJump()
{
	if (!crouching)
		offsetY = 2.5f;
	else
		offsetY = 1.5f;
	jumpDown = false;
	jumping = false;
}

void Camera::pitch(float angle)
{
	D3DXMATRIX R;
	D3DXMatrixRotationAxis(&R, &mRight, angle);

	D3DXVec3TransformNormal(&mUp, &mUp, &R);
	D3DXVec3TransformNormal(&mLook, &mLook, &R);
}

void Camera::rotateY(float angle)
{
	D3DXMATRIX R;
	D3DXMatrixRotationY(&R, angle);

	D3DXVec3TransformNormal(&mRight, &mRight, &R);
	D3DXVec3TransformNormal(&mUp, &mUp, &R);
	D3DXVec3TransformNormal(&mLook, &mLook, &R);
}

void Camera::keyMsg(float dt)
{
	if(GetAsyncKeyState('A') & 0x8000)	strafe(-18.0f*dt);
	if(GetAsyncKeyState('D') & 0x8000)	strafe(18.0f*dt);
	if(GetAsyncKeyState('W') & 0x8000)	walk(18.0f*dt);
	if(GetAsyncKeyState('S') & 0x8000)	walk(-18.0f*dt);
}

void Camera::rebuildView()
{
	// Keep camera's axes orthogonal to each other and of unit length.
	D3DXVec3Normalize(&mLook, &mLook);

	D3DXVec3Cross(&mUp, &mLook, &mRight);
	D3DXVec3Normalize(&mUp, &mUp);

	D3DXVec3Cross(&mRight, &mUp, &mLook);
	D3DXVec3Normalize(&mRight, &mRight);

	// Fill in the view matrix entries.
	float x = -D3DXVec3Dot(&mPosition, &mRight);
	float y = -D3DXVec3Dot(&mPosition, &mUp);
	float z = -D3DXVec3Dot(&mPosition, &mLook);

	mView._11 = mRight.x; 
	mView._21 = mRight.y; 
	mView._31 = mRight.z; 
	mView._41 = x;   

	mView._12 = mUp.x;
	mView._22 = mUp.y;
	mView._32 = mUp.z;
	mView._42 = y;  

	mView._13 = mLook.x; 
	mView._23 = mLook.y; 
	mView._33 = mLook.z; 
	mView._43 = z;   

	mView._14 = 0.0f;
	mView._24 = 0.0f;
	mView._34 = 0.0f;
	mView._44 = 1.0f;
}