#ifndef CYLINDER_H
#define CYLINDER_H

#include <d3dx10.h>
#include <vector>
#include "Vertex.h"

class Cylinder
{
private:
	D3DXMATRIX mWorld;
	D3DXVECTOR3 pos;
	ID3D10Device* d3dDevice;
	ID3D10Buffer* vertexBuffer;
	ID3D10Buffer* indexBuffer;
	DWORD nrOfVertices;
	std::vector<Vertex> v;
public:
	Cylinder();
	~Cylinder();
	void init(ID3D10Device* d3dDevice, float size, D3DXVECTOR3 pos);
	void move(D3DXVECTOR3 newPos);
	void Draw();
	D3DXMATRIX& getWorld();
};
#endif