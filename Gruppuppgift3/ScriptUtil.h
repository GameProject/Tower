#ifndef SCRIPTUTIL_H
#define SCRIPTUTIL_H

#include <d3dx10.h>
#include <string>

struct attackInfo
{
	attackInfo()
	{
		dmg = -1;
		eID = -1;
		magic = "-";
	}
	int dmg;
	int eID;
	std::string magic;
};

struct enemyInfo
{
	enemyInfo()
	{
		ePos = D3DXVECTOR3(-99,-99,-99);
		eID = -1;
		eRadius = -1;
	}
	int eID;
	D3DXVECTOR3 ePos;
	float eRadius;
};

#endif