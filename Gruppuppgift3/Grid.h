#ifndef GRID_H
#define GRID_H

#include "Vertex.h"
#include "BoxNode.h"
#include "Effects.h"
#include "Terrain.h"

//ID3DX10Mesh
#include <dxerr.h>

class Grid
{
private:
	ID3D10Device*				device;
	ID3D10EffectTechnique*		tech;
	ID3D10EffectMatrixVariable* mfxWorldVar;
	ID3D10EffectMatrixVariable* mfxWVPVar;

	D3DXMATRIX World;
	ID3D10Buffer* vb;
	ID3D10Buffer* ib;
	int nrOfFaces;

	Terrain* pLand;

	std::vector<GridVertex> v;
	std::vector<DWORD> indices;
	std::vector<float> y;
	ID3DX10Mesh* mMeshData;
public:
	Grid();
	ID3DX10Mesh* d3dxMesh();
	void getAABB(D3DXVECTOR3& vMin, D3DXVECTOR3& vMax);
	void setLandRef(Terrain *land);
	void initGrid(ID3D10Device* d);
	void LoadGrid(BoxNode &sRoot, D3DXVECTOR4 color);
	void drawGrid(D3DXMATRIX view, D3DXMATRIX proj);
	std::vector<GridVertex> GetVertices();
	std::vector<DWORD> GetIndices();
	int GetNumFaces();
	D3DXMATRIX& getWorld();
};
#endif