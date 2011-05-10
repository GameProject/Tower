#include "QuadTree.h"
#include "Camera.h"

QuadTree::QuadTree()
{}

void QuadTree::init(ID3D10Device* device, Terrain* land, D3DXMATRIX view, D3DXMATRIX proj)
{
	pLand = land;
	root.minB = D3DXVECTOR3(-255, pLand->getHeight(-255, -255), -255);
	root.maxB = D3DXVECTOR3(255, pLand->getHeight(255, 255), 255);

	grid.setLandRef(pLand);

	int n = 5;
	createTree(n, root);
	grid.initGrid(device);
	newTowerPos = D3DXVECTOR3(-99,-99,-99);
}

void QuadTree::createTree(int nrOfLayers, BoxNode &sRoot)
{

	if(nrOfLayers != 0)
	{
		BoxNode temp, temp1, temp2, temp3;
		for(int i = 0; i < 4; i++)
		{
			if(i == 0)
			{
				temp.minB = sRoot.minB;
				temp.maxB = (sRoot.minB + sRoot.maxB)/2;
				temp.maxB.y = sRoot.maxB.y;
				sRoot.ChildNode.push_back(temp);
			}
			if(i == 1)
			{
				temp1.maxB = D3DXVECTOR3(sRoot.maxB.x,sRoot.maxB.y,(sRoot.minB.z + sRoot.maxB.z)/2);
				temp1.minB = D3DXVECTOR3((sRoot.minB.x + sRoot.maxB.x)/2,sRoot.minB.y,sRoot.minB.z);
				sRoot.ChildNode.push_back(temp1);
			}
			if(i == 2)
			{
				temp2.minB = D3DXVECTOR3(sRoot.minB.x,sRoot.minB.y,(sRoot.minB.z + sRoot.maxB.z)/2);
				temp2.maxB = D3DXVECTOR3((sRoot.minB.x + sRoot.maxB.x)/2,sRoot.maxB.y,sRoot.maxB.z);
				sRoot.ChildNode.push_back(temp2);
			}
			if(i == 3)
			{
				temp3.minB = (sRoot.minB + sRoot.maxB)/2;
				temp3.minB.y = sRoot.minB.y;
				temp3.maxB = sRoot.maxB;
				sRoot.ChildNode.push_back(temp3);
			}
			
		}
		for (int i=0; i<4; ++i)
			this->createTree(nrOfLayers-1, sRoot.ChildNode.at(i));
	}
	if(nrOfLayers == 1)
		for (int i=0; i<4; ++i)
			this->LoadVB(sRoot.ChildNode.at(i));
}

void QuadTree::LoadVB(BoxNode &sRoot)
{
	D3DXVECTOR4 red(1.0f,0.0f,0.0f,1.0f);
	grid.LoadGrid(sRoot, red);
}
void QuadTree::draw(D3DXMATRIX view, D3DXMATRIX proj, bool drawGrid)
{	
	if (drawGrid)
		grid.drawGrid(view, proj);
}

BoxNode QuadTree::getBLS()
{
	return root;
}

D3DXVECTOR3 QuadTree::PlaceATower(D3D10_VIEWPORT vp, POINT mouse)
{
	D3DXVECTOR3 vPickRayDir;
	D3DXVECTOR3 vPickRayOrig;
	GenerateMouse3DPos(vp, mouse, vPickRayDir, vPickRayOrig);

	SearchTree(vPickRayDir, vPickRayOrig, getBLS());

	D3DXVECTOR3 ret = newTowerPos;
	newTowerPos = D3DXVECTOR3(-99,-99,-99);
	return ret;
}

void QuadTree::GenerateMouse3DPos(D3D10_VIEWPORT vp, POINT mouse, D3DXVECTOR3 &vPickRayDir, D3DXVECTOR3 &vPickRayOrig)
{
	D3DXMATRIX pmatProj = GetCamera().proj();

	// Compute the vector of the pick ray in screen space
	D3DXVECTOR3 v;
	v.x = ( ( ( 2.0f * mouse.x ) / vp.Width ) - 1 ) / pmatProj._11;
	v.y = -( ( ( 2.0f * mouse.y ) / vp.Height ) - 1 ) / pmatProj._22;
	v.z = 1.0f;

	// Get the inverse view matrix
	D3DXMATRIX matView = GetCamera().view();
	D3DXMATRIX matWorld = grid.getWorld();
	D3DXMATRIX mWorldView = matWorld * matView;
	D3DXMATRIX m;
	D3DXMatrixInverse( &m, NULL, &mWorldView );

	// Transform the screen space pick ray into 3D space
	vPickRayDir.x = v.x * m._11 + v.y * m._21 + v.z * m._31;
	vPickRayDir.y = v.x * m._12 + v.y * m._22 + v.z * m._32;
	vPickRayDir.z = v.x * m._13 + v.y * m._23 + v.z * m._33;
	vPickRayOrig.x = m._41;
	vPickRayOrig.y = m._42;
	vPickRayOrig.z = m._43;
}

void QuadTree::SearchTree(D3DXVECTOR3 vPickRayDir, D3DXVECTOR3 vPickRayOrig, BoxNode box)
{
	bool result = false;

	result = Pick(vPickRayDir, vPickRayOrig, box);

	if (!result)
		return;

	if (!box.ChildNode.empty())
	{	
		for (int i=0; i<4; ++i)
			SearchTree(vPickRayDir, vPickRayOrig, box.ChildNode.at(i));
		return;
	}

	newTowerPos.x = (box.minB.x+box.maxB.x)/2;
	newTowerPos.z = (box.minB.z+box.maxB.z)/2;
	newTowerPos.y =  pLand->getHeight(newTowerPos.x, newTowerPos.z)+1;
	return;
}

bool QuadTree::Pick(D3DXVECTOR3 vPickRayDir, D3DXVECTOR3 vPickRayOrig, BoxNode box)
{
	/*
		|\
		| \
		|__\	*/
	D3DXVECTOR3 v0 = box.minB;
	D3DXVECTOR3 v1 = D3DXVECTOR3(box.minB.x, pLand->getHeight(box.minB.x, box.maxB.z)+1, box.maxB.z);
	D3DXVECTOR3 v2 = D3DXVECTOR3(box.maxB.x, pLand->getHeight(box.maxB.x, box.minB.z)+1, box.minB.z);

	/*	___
		\  |
		 \ |
		  \|	*/
	D3DXVECTOR3 v3 = v1;
	D3DXVECTOR3 v4 = v2;
	D3DXVECTOR3 v5 = box.maxB;

	//Check if the pick ray passes through triangles
	if( IntersectTriangle( vPickRayOrig, vPickRayDir, v0, v1, v2) )
		return true;
	else if( IntersectTriangle( vPickRayOrig, vPickRayDir, v3, v4, v5) )
		return true;

	return false;
}


//--------------------------------------------------------------------------------------
// Given a ray origin (orig) and direction (dir), and three vertices of a triangle, this
// function returns TRUE and the interpolated texture coordinates if the ray intersects 
// the triangle
//--------------------------------------------------------------------------------------
bool QuadTree::IntersectTriangle( const D3DXVECTOR3& orig, const D3DXVECTOR3& dir,
					   D3DXVECTOR3& v0, D3DXVECTOR3& v1, D3DXVECTOR3& v2)
{
	FLOAT t, u, v;

	// Find vectors for two edges sharing vert0
	D3DXVECTOR3 edge1 = v1 - v0;
	D3DXVECTOR3 edge2 = v2 - v0;

	// Begin calculating determinant - also used to calculate U parameter
	D3DXVECTOR3 pvec;
	D3DXVec3Cross( &pvec, &dir, &edge2 );

	// If determinant is near zero, ray lies in plane of triangle
	FLOAT det = D3DXVec3Dot( &edge1, &pvec );

	D3DXVECTOR3 tvec;
	if( det > 0 )
	{
		tvec = orig - v0;
	}
	else
	{
		tvec = v0 - orig;
		det = -det;
	}

	if( det < 0.0001f )
		return FALSE;

	// Calculate U parameter and test bounds
	u = D3DXVec3Dot( &tvec, &pvec );
	if( u < 0.0f || u > det )
		return FALSE;

	// Prepare to test V parameter
	D3DXVECTOR3 qvec;
	D3DXVec3Cross( &qvec, &tvec, &edge1 );

	// Calculate V parameter and test bounds
	v = D3DXVec3Dot( &dir, &qvec );
	if( v < 0.0f || u + v > det )
		return FALSE;

	// Calculate t, scale parameters, ray intersects triangle
	t = D3DXVec3Dot( &edge2, &qvec );
	FLOAT fInvDet = 1.0f / det;
	t *= fInvDet;
	u *= fInvDet;
	v *= fInvDet;

	return TRUE;
}