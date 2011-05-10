#ifndef PROJECTILE_H
#define PROJECTILE_H

#include <d3dx10.h>
#include <vector>
#include "Vertex.h"

class Projectile
{
private:
	D3DXMATRIX mWorld;
	D3DXMATRIX mRot;
	D3DXMATRIX mScale;
	D3DXVECTOR3 pos;
	ID3D10Device* d3dDevice;
	ID3D10Buffer* vertexBuffer;
	ID3D10Buffer* indexBuffer;
	DWORD nrOfVertices, nrOfFaces;
	std::vector<Vertex> v;
	int texNr;
public:
	Projectile();
	void release();
	void init(ID3D10Device* d3dDevice, D3DXVECTOR3 size, D3DXVECTOR3 pos);
	void rotate(float t);
	void move(D3DXVECTOR3 enemyPos);
	void scaleProjectile(float s);
	void Draw();
	void setUV(float d, bool alongX);
	void setTexNr(int nr);
	int  getTexNr();
	D3DXMATRIX& getWorld();
};
#endif