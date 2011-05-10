#ifndef LIGHT_H
#define LIGHT_H

#include <d3dx10.h>

struct Light
{
	Light()
	{
		pos		 = D3DXVECTOR3(-2.0f, 3.5f, -8.0f);
		dir		 = D3DXVECTOR3(0.5f, -0.5f, 0.5f);
		ambient	 = D3DXCOLOR(0.4f, 0.3f, 0.1f, 1.0f);
		diffuse	 = D3DXCOLOR(1.0f, 0.8f, 0.4f, 1.0f);
		specular = D3DXCOLOR(1.0f, 1.0f, 0.4f, 1.0f);
		att.x = 0.5f;
		att.y = 0.0314098f;
		att.z = 0.0314098f;
		spotPow = 50.0f;
		range   = 1000.0f;
		lightType = 2;
	}

// 	float RandF()
// 	{
// 		return (float)(rand()) / (float)RAND_MAX;
// 	}
// 
// 	// Returns random float in [a, b].
// 	float RandF(float a, float b)
// 	{
// 		return a + RandF()*(b-a);
// 	}

	void rot(float t, float _dt, D3DXVECTOR3 target, D3DXVECTOR3 eyePos)
	{
		if (lightType == 1)
		{
			//DirectionalLight
			D3DXVECTOR3 DirLightDirection;
			DirLightDirection.x = cosf(0.5f*t);
			DirLightDirection.y = -0.5f;
			DirLightDirection.z = sinf(0.5f*t);
			D3DXVec3Normalize(&dir, &DirLightDirection);
		}
		else if (lightType == 2)
		{
			//PointLight
			//Flicker On/Off
// 			n = RandF(0, 4);
// 			if (n < 2.0f)
// 			{
// 				if (att.y < 0.2088f && att.z < 0.0313f)
// 				{
// 					att.y += 0.5f*_dt;
// 					att.z += 0.5f*_dt;
// 				}
// 			}
// 			else if (n >= 1.0f && n <= 3.0f)
// 			{
// 				if (att.y < 0.07f)
// 				{
// 					if (att.y < 0.0888f && att.z < 0.0163f)
// 					{
// 						att.y += 0.5f*_dt;
// 						att.z += 0.5f*_dt;
// 					}
// 				}
// 				else
// 				{
// 					if (att.y > 0.0888f && att.z > 0.0163f)
// 					{
// 						att.y -= 0.5f*_dt;
// 						att.z -= 0.5f*_dt;
// 					}
// 				}
// 			}
// 			else
// 			{
// 				if (att.y > 0.1988f && att.z > 0.0273f)
// 				{
// 					att.y -= 0.5f*_dt;
// 					att.z -= 0.5f*_dt;
// 				}
// 			}

			//Rotation
// 			pos.x = 15.0f*cosf(t);
// 			pos.z = 15.0f*sinf(t);
// 			pos.y = 10.0f;
		}
		else
		{
			//SpotLight
			pos = eyePos;
			D3DXVec3Normalize(&dir, &(target));
		}
	}

	void keyMsg(float dt)
	{
		if(GetAsyncKeyState('T') & 0x8000) att.x += 0.05f*dt;
		if(GetAsyncKeyState('Y') & 0x8000) att.y += 0.05f*dt;
		if(GetAsyncKeyState('U') & 0x8000) att.z += 0.05f*dt;
		if(GetAsyncKeyState('G') & 0x8000) att.x -= 0.05f*dt;
		if(GetAsyncKeyState('H') & 0x8000) att.y -= 0.05f*dt;
		if(GetAsyncKeyState('J') & 0x8000) att.z -= 0.05f*dt;

		if(GetAsyncKeyState('1') & 0x8000) 
		{
			att.x = 0.0f;
			att.y = 0.1f;
			att.z = 0.0f;
			ambient	 = D3DXCOLOR(0.4f, 0.4f, 0.4f, 1.0f);
			diffuse	 = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			specular = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			lightType = 1;
		}
		if(GetAsyncKeyState('2') & 0x8000) 
		{
			att.x = 0.5f;
			att.y = 0.0314098f;
			att.z = 0.0314098f;
			pos.x = -2.0f;
			pos.y = 3.5f;
			pos.z = -8.0f;
			ambient	 = D3DXCOLOR(0.4f, 0.3f, 0.1f, 1.0f);
			diffuse	 = D3DXCOLOR(1.0f, 0.8f, 0.4f, 1.0f);
			specular = D3DXCOLOR(1.0f, 1.0f, 0.4f, 1.0f);
			lightType = 2;
		}
		if(GetAsyncKeyState('3') & 0x8000) 
		{
			att.x = 0.5f;
			att.y = 0.0f;
			att.z = 0.0f;
			ambient	 = D3DXCOLOR(0.4f, 0.4f, 0.4f, 1.0f);
			diffuse	 = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			specular = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			lightType = 3;
		}
	}

	D3DXVECTOR3 pos;
	float pad1;
	D3DXVECTOR3 dir;
	float pad2;
	D3DXCOLOR ambient;
	D3DXCOLOR diffuse;
	D3DXCOLOR specular;
	D3DXVECTOR3 att;
	float spotPow;
	float range;
	int lightType;
	//float n;
};
#endif