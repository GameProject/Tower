#ifndef QUADTREE_H
#define QUADTREE_H

#include "Grid.h"
#include "Terrain.h"
#include "RayUtil.h"

using namespace std;

class QuadTree
{
private:
	BoxNode root;
	Grid grid;
	Terrain* pLand;
	D3DXVECTOR3 newTowerPos;
	int g_nNumIntersections;
public:
	QuadTree();
	void init(ID3D10Device* device, Terrain* land, D3DXMATRIX view, D3DXMATRIX proj);
	void createTree(int nrOfLayers, BoxNode &sRoot);
	void LoadVB(BoxNode &sRoot);
	void draw(D3DXMATRIX view, D3DXMATRIX proj, bool drawGrid);
	BoxNode getBLS();

	D3DXVECTOR3 PlaceATower(D3D10_VIEWPORT vp, POINT mouse);
	void GenerateMouse3DPos(D3D10_VIEWPORT vp, POINT mouse, D3DXVECTOR3 &vPickRayDir, D3DXVECTOR3 &vPickRayOrig);
	void SearchTree(D3DXVECTOR3 vPickRayDir, D3DXVECTOR3 vPickRayOrig, BoxNode box);
	bool Pick(D3DXVECTOR3 vPickRayDir, D3DXVECTOR3 vPickRayOrig, BoxNode box);
	bool IntersectTriangle( const D3DXVECTOR3& orig, const D3DXVECTOR3& dir,
		D3DXVECTOR3& v0, D3DXVECTOR3& v1, D3DXVECTOR3& v2);
};
#endif