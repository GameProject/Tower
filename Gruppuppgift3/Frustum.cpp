#include "Frustum.h"

Frustum::Frustum(){}

void Frustum::UpdateFrustum(D3DXMATRIX V, D3DXMATRIX P)
{
	D3DXMATRIX VP = V*P;
	D3DXVECTOR4 col0(VP(0,0), VP(1,0), VP(2,0), VP(3,0));
	D3DXVECTOR4 col1(VP(0,1), VP(1,1), VP(2,1), VP(3,1));
	D3DXVECTOR4 col2(VP(0,2), VP(1,2), VP(2,2), VP(3,2));
	D3DXVECTOR4 col3(VP(0,3), VP(1,3), VP(2,3), VP(3,3));
	
	// Planes face inward.
	// near	
	m_planes[0] = (D3DXPLANE)(col2);
	// far
	m_planes[1] = (D3DXPLANE)(col3 - col2);
	// left	
	m_planes[2] = (D3DXPLANE)(col3 + col0);
	// right		
	m_planes[3] = (D3DXPLANE)(col3 - col0);
	// top				
	m_planes[4] = (D3DXPLANE)(col3 - col1);
	// bottom		
	m_planes[5] = (D3DXPLANE)(col3 + col1);
			
	for(int i = 0; i < 6; i++)				
		D3DXPlaneNormalize(&m_planes[i], &m_planes[i]);
}

bool Frustum::BoxFrustumIntersect(BoxNode B)
{	
	D3DXVECTOR3 Q;	
	for(int i = 0; i < 6; ++i)	
	{
		for(int j=0; j<3; ++j)		
		{			
			if(m_planes[i][j] >= 0.0f)						
				Q[j] = B.maxB[j];					
			else 						
				Q[j] = B.minB[j];
		}		
		float kne = D3DXPlaneDotCoord(&m_planes[i], &Q);		
		if (kne < 0) 
			return false;	
	}	
	return true;
}