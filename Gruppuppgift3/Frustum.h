#ifndef FRUSTUM_H
#define FRUSTUM_H

#include "Obj.h"
#include "Camera.h"
#include "BoxNode.h"
#include <d3dx10math.h>
#include "Grid.h"

class Frustum
{
private:
	D3DXPLANE m_planes[6];
public:
	Frustum();
	void UpdateFrustum(D3DXMATRIX V, D3DXMATRIX P);
	bool BoxFrustumIntersect(BoxNode B);
};
#endif