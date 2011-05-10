#ifndef HEIGHTMAP_H
#define HEIGHTMAP_H

#include "Vertex.h"
#include <vector>
#include <string>

class Heightmap
{
private:
	D3DXMATRIX mWorld;
	ID3D10Device* d3dDevice;
	ID3D10Buffer* vertexBuffer;
	ID3D10Buffer* indexBuffer;
	std::vector<float> mHeightmap;
	float mHeightScale;
	float mHeightOffset;
	float mSize;
	UINT mNumCols;
	UINT mNumRows;
	UINT mNumVertices;
	UINT mNumFaces;
	std::vector<TerrainVertex> vpnt;
	float lastY;
	D3DXVECTOR3 minPos;
	D3DXVECTOR3 maxPos;
public:
	Heightmap();
	~Heightmap();
	void init(ID3D10Device* device, 
				std::string& filename, 
				float HeightScale, 
				float HeightOffset, 
				float size,
				int NumRows, int NumCols);
	void boxFilter();
	float getAverage(UINT i, UINT j);
	bool inBoundsOfHeightMap(UINT i, UINT j);
	void CalculateTextureCoordinates();
	void buildVB();
	void buildIB();
	float getPosY(float x, float z);
	float getHeight(float x, float z);
	void Draw();
	D3DXMATRIX& getWorld();
	int GetVertexCount();
	std::vector<TerrainVertex> CopyVertexArray();
	void DoMinMax();
	D3DXVECTOR3 getMin();
	D3DXVECTOR3 getMax();
};
#endif