#ifndef CAMERA_H
#define CAMERA_H

#include <d3dx10.h>

class Camera
{
private:
	D3DXVECTOR3 mPosition;
	D3DXVECTOR3 mRight;
	D3DXVECTOR3 mUp;
	D3DXVECTOR3 mLook;
	D3DXMATRIX mView;
	D3DXMATRIX mProj;
	bool crouching;
	float offsetY;
	float jumpMAX;
	float jumpMIN;
	bool jumpDown;
	bool jumping;
	float start;
	float mLastDeltaTime;

public:
	Camera();
	D3DXVECTOR3 getPos();
	D3DXVECTOR3 getLook();
	D3DXMATRIX view()const;
	D3DXMATRIX proj()const;
	D3DXMATRIX wvp(D3DXMATRIX world);
	void setPosY(float y);
	void setPos(D3DXVECTOR3 pos);
	void setLens(float fovY, float aspect, float zn, float zf);
	void strafe(float dt);
	void walk(float dt);
	void crouch();
	void setJump(float t);
	bool jump(float t);
	void stopJump();
	void pitch(float angle);
	void rotateY(float angle);
	void keyMsg(float dt);
	void rebuildView();
};
Camera& GetCamera();
#endif