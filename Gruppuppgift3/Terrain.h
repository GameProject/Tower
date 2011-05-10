#ifndef TERRAIN_H
#define TERRAIN_H

#include <windows.h>
#include <d3dx10.h>
#include <string>
#include <vector>
#include <fstream>
#include "Vertex.h"

using namespace std;

class Terrain
{
	public:
	struct InitInfo
	{
		std::wstring HeightmapFilename;
		float HeightScale;
		float HeightOffset;
		UINT NumRows;
		UINT NumCols;
		float CellSpacing;
	};

private:
	InitInfo mInfo;
	D3DXMATRIX mWorld;
	UINT pNumVertices;
	UINT pNumFaces;

	std::vector<float> mHeightmap;
	std::vector<TerrainVertex> vertices;

	ID3D10Device* pd3dDevice;
	ID3D10Buffer* pVB;
	ID3D10Buffer* pIB;

	D3DXVECTOR3 minPos;
	D3DXVECTOR3 maxPos;
public:
	Terrain();
	~Terrain();

	float width()const;
	float depth()const;
	float getHeight(float x, float z)const;

	void init(ID3D10Device* device, const InitInfo& initInfo);
	void DoMinMax();
	D3DXVECTOR3 getMin();
	D3DXVECTOR3 getMax();

	void setDirectionToSun(const D3DXVECTOR3& v);
	D3DXMATRIX& getWorld();
	void draw();
private:
	void loadHeightmap();
	void smooth();

	bool inBounds(UINT i, UINT j);
	float average(UINT i, UINT j);

	void buildVB();
	void buildIB();
};
#endif