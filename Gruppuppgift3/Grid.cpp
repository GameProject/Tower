#include "Grid.h"
#include <fstream>
#include <sstream>
Grid::Grid()
{
	 mMeshData = 0;
}

void Grid::setLandRef(Terrain *land)
{
	pLand = land;
}

ID3DX10Mesh* Grid::d3dxMesh()
{
	return mMeshData;
}

void Grid::initGrid(ID3D10Device* d)
{
	//std::ofstream myfile;
	//myfile.open("gridY.txt");
	//for (int i=0; i<y.size(); ++i)	
	//	myfile << y.at(i) << "\n";
	//myfile.close();

	device = d;

	D3DXMatrixIdentity(&World);
	device = device;
	tech = Effects::AABBFX->GetTechniqueByName("Render");
	mfxWVPVar = Effects::AABBFX->GetVariableByName( "WVP")->AsMatrix();
	mfxWorldVar = Effects::AABBFX->GetVariableByName("World")->AsMatrix();

// 	D3D10_INPUT_ELEMENT_DESC vertexDesc[] =
// 	{
// 		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0,  D3D10_INPUT_PER_VERTEX_DATA, 0},
// 		{"COLOR",   0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D10_INPUT_PER_VERTEX_DATA, 0}
// 	};
// 	D3DX10CreateMesh(device, vertexDesc, 2, 
// 		vertexDesc[0].SemanticName, (int)v.size(), 
// 		(int)v.size()/2, D3DX10_MESH_32_BIT, &mMeshData);

	D3D10_BUFFER_DESC vbd;
	vbd.Usage = D3D10_USAGE_IMMUTABLE;
	vbd.ByteWidth = sizeof(GridVertex) * (UINT)v.size();
	vbd.BindFlags = D3D10_BIND_VERTEX_BUFFER;
	vbd.CPUAccessFlags = 0;
	vbd.MiscFlags = 0;

	D3D10_SUBRESOURCE_DATA vinitData;
	vinitData.pSysMem = &v[0];
	device->CreateBuffer(&vbd, &vinitData, &vb);

 	indices.resize(v.size());
 	UINT* attributeBuffer = new UINT[v.size()];
//  GridVertex* verts = new GridVertex[v.size()];

	for(int i = 0; i < (int)v.size();i++)
		attributeBuffer[i] = i;//indices.push_back(i);
 
//  	for(int i = 0; i < (int)v.size();i++)
//  	{
//  		indices[i*2+0] = i;
//  		indices[i*2+1] = i;
// 			attributeBuffer[i] = 0;
//  		verts[i] = v.at(i);
//  	}
//  
//  	mMeshData->SetVertexData(0, verts);
//  	mMeshData->SetIndexData(indices, (UINT)v.size()/2);
//  	mMeshData->SetAttributeData(attributeBuffer);
//  
//  	mMeshData->GenerateAdjacencyAndPointReps(0.001f);
//  	mMeshData->Optimize(D3DX10_MESHOPT_ATTR_SORT|D3DX10_MESHOPT_VERTEX_CACHE,0,0);
//  	mMeshData->CommitToDevice();

	D3D10_BUFFER_DESC ibd;
	ibd.Usage = D3D10_USAGE_IMMUTABLE;
	ibd.ByteWidth = sizeof(DWORD) * (UINT)v.size();
	ibd.BindFlags = D3D10_BIND_INDEX_BUFFER;
	ibd.CPUAccessFlags = 0;
	ibd.MiscFlags = 0;

	D3D10_SUBRESOURCE_DATA iinitData;
	iinitData.pSysMem = &attributeBuffer[0];
	device->CreateBuffer(&ibd, &iinitData, &ib);
}

void Grid::LoadGrid(BoxNode &sRoot, D3DXVECTOR4 color)
{
	v.push_back(GridVertex(D3DXVECTOR3(sRoot.minB.x, pLand->getHeight(sRoot.minB.x, sRoot.minB.z)+1, sRoot.minB.z),color));
	v.push_back(GridVertex(D3DXVECTOR3(sRoot.maxB.x, pLand->getHeight(sRoot.maxB.x, sRoot.minB.z)+1, sRoot.minB.z),color));

	v.push_back(GridVertex(D3DXVECTOR3(sRoot.minB.x, pLand->getHeight(sRoot.minB.x, sRoot.maxB.z)+1, sRoot.maxB.z),color));
	v.push_back(GridVertex(D3DXVECTOR3(sRoot.maxB.x, pLand->getHeight(sRoot.maxB.x, sRoot.maxB.z)+1, sRoot.maxB.z),color));

	v.push_back(GridVertex(D3DXVECTOR3(sRoot.minB.x, pLand->getHeight(sRoot.minB.x, sRoot.minB.z)+1, sRoot.minB.z),color));
	v.push_back(GridVertex(D3DXVECTOR3(sRoot.minB.x, pLand->getHeight(sRoot.minB.x, sRoot.maxB.z)+1, sRoot.maxB.z),color));

	v.push_back(GridVertex(D3DXVECTOR3(sRoot.maxB.x, pLand->getHeight(sRoot.maxB.x, sRoot.minB.z)+1, sRoot.minB.z),color));
	v.push_back(GridVertex(D3DXVECTOR3(sRoot.maxB.x, pLand->getHeight(sRoot.maxB.x, sRoot.maxB.z)+1, sRoot.maxB.z),color));

	nrOfFaces += 2;

	//y.push_back(sRoot.Y[0]+1);
	//y.push_back(sRoot.Y[1]+1);
	//y.push_back(sRoot.Y[2]+1);
	//y.push_back(sRoot.Y[3]+1);
}

void Grid::getAABB(D3DXVECTOR3& vMin, D3DXVECTOR3& vMax)
{
	ID3DX10MeshBuffer* vb = 0;
	mMeshData->GetVertexBuffer(0, &vb);

	GridVertex* vertices = 0;
	SIZE_T size;
	vb->Map((void**)&vertices, &size);

	vMin = D3DXVECTOR3(+FLT_MAX, +FLT_MAX, +FLT_MAX);
	vMax = D3DXVECTOR3(-FLT_MAX, -FLT_MAX, -FLT_MAX);

	for(UINT i = 0; i < mMeshData->GetVertexCount(); ++i)
	{
		D3DXVec3Minimize(&vMin, &vMin, &vertices[i].pos);
		D3DXVec3Maximize(&vMax, &vMax, &vertices[i].pos);
	}

	vb->Unmap();
	vb->Release();
}

void Grid::drawGrid(D3DXMATRIX view, D3DXMATRIX proj)
{
	device->IASetInputLayout(Effects::PosColor);

	UINT stride = sizeof(GridVertex);
	UINT offset = 0;
	device->IASetVertexBuffers(0, 1, &vb, &stride, &offset);
	device->IASetIndexBuffer(ib, DXGI_FORMAT_R32_UINT, 0);
	device->IASetPrimitiveTopology(D3D10_PRIMITIVE_TOPOLOGY_LINELIST);

	mfxWorldVar->SetMatrix(World);
	mfxWVPVar->SetMatrix(World*view*proj);

	D3D10_TECHNIQUE_DESC techDesc;
	tech->GetDesc(&techDesc);
	for(UINT p = 0; p < (UINT)techDesc.Passes; ++p)
	{
		this->tech->GetPassByIndex(p)->Apply(0);
		this->device->DrawIndexed((UINT)v.size(), 0, 0);
		//mMeshData->DrawSubset(0);
	}
}

std::vector<GridVertex> Grid::GetVertices()
{
	return v;
}

std::vector<DWORD> Grid::GetIndices()
{
	return indices;
}

int Grid::GetNumFaces()
{
	return nrOfFaces;
}

D3DXMATRIX& Grid::getWorld()
{
	return World;
}