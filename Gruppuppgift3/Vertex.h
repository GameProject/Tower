#ifndef VERTEX_H
#define VERTEX_H

#include <d3dx10.h>

struct Vertex
{
	Vertex(){}
	Vertex(float x, float y, float z, float nx, float ny, float nz, float u, float v, float tx, float ty, float tz)
		:pos(x,y,z), normal(nx,ny,nz), texC(u,v), tengent(tx, ty, tz){}

	D3DXVECTOR3 pos;
	D3DXVECTOR3 normal;
	D3DXVECTOR2 texC;
	D3DXVECTOR3 tengent;
};

struct TerrainVertex
{
	TerrainVertex(){};
	TerrainVertex(D3DXVECTOR3 _pos, D3DXVECTOR3 _normal, D3DXVECTOR2 _texC)
	{
		pos = _pos;
		normal = _normal;
		texC = _texC;
	}

	D3DXVECTOR3 pos;
	D3DXVECTOR3 normal;
	D3DXVECTOR2 texC;
};

struct ParticleVertex
{
	D3DXVECTOR3 pos;
	D3DXVECTOR3 vel;
	D3DXVECTOR2 size;
	float age;
	int type;
};

struct BillboardVertex
{
	D3DXVECTOR3 centerW;
	D3DXVECTOR2 sizeW;
};

struct GridVertex
{
	D3DXVECTOR3 pos;
	D3DXVECTOR4 color;
	GridVertex()
	{
	}
	GridVertex(float x, float y, float z, D3DXVECTOR4 _color)
		:pos(x,y,z), color(_color){}
	GridVertex(D3DXVECTOR3 _pos, D3DXVECTOR4 _color)
	{
		pos = _pos;
		color = _color;
	}
};

#endif