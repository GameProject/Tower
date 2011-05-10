#include "Pyramid.h"
#include "Vertex.h"

Pyramid::Pyramid()
{
	d3dDevice = 0; 
	vertexBuffer = 0;
	indexBuffer = 0;
	nrOfVertices = 5;
}

Pyramid::~Pyramid()
{
	vertexBuffer->Release();
	indexBuffer->Release();
	vertexBuffer = 0;
	indexBuffer  = 0;
}

void Pyramid::init(ID3D10Device *device, D3DXVECTOR3 size, D3DXVECTOR3 pos)
{
	d3dDevice = device;
	D3DXMatrixIdentity(&mWorld);

	// Create vertex buffer
	v.resize(nrOfVertices);

	// Fill in the face vertex data.
	//base
	v[0] = Vertex(-size.x, -size.y, -size.z, 0.0f, -1.0f, 0.0f, 1.0f, 1.0f, -1.0f, 0.0f, 0.0f);
	v[1] = Vertex( size.x, -size.y, -size.z, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f, -1.0f, 0.0f, 0.0f);
	v[2] = Vertex( size.x, -size.y,  size.z, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f);
	v[3] = Vertex(-size.x, -size.y,  size.z, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f, -1.0f, 0.0f, 0.0f);

	//top
	v[4] = Vertex(0.0f, size.z, 0.0f, 0.0f, 1.0f, 0.0f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f);

	for(DWORD i = 0; i < 5; ++i)
	{
		v[i].texC.x *= size.x;
		v[i].texC.y *= size.y;
	}
	move(pos);

	D3D10_BUFFER_DESC vbd;
	vbd.Usage = D3D10_USAGE_IMMUTABLE;
	vbd.ByteWidth = sizeof(Vertex) * nrOfVertices;
	vbd.BindFlags = D3D10_BIND_VERTEX_BUFFER;
	vbd.CPUAccessFlags = 0;
	vbd.MiscFlags = 0;
	D3D10_SUBRESOURCE_DATA vinitData;
	vinitData.pSysMem = &v[0];
	d3dDevice->CreateBuffer(&vbd, &vinitData, &vertexBuffer);


	// Create the index buffer
	DWORD i[18];
	// Fill in the face index data
	//base
	i[0]  = 0; i[1]  = 1; i[2]  = 2;
	i[3]  = 0; i[4]  = 2; i[5]  = 3;
	//sides
	i[6]  = 0; i[7]  = 4; i[8]  = 1;
	i[9]  = 1; i[10] = 4; i[11] = 2;
	i[12] = 2; i[13] = 4; i[14] = 3;
	i[15] = 3; i[16] = 4; i[17] = 0;

	D3D10_BUFFER_DESC ibd;
	ibd.Usage = D3D10_USAGE_IMMUTABLE;
	ibd.ByteWidth = sizeof(DWORD)*18;
	ibd.BindFlags = D3D10_BIND_INDEX_BUFFER;
	ibd.CPUAccessFlags = 0;
	ibd.MiscFlags = 0;
	D3D10_SUBRESOURCE_DATA iinitData;
	iinitData.pSysMem = i;
	d3dDevice->CreateBuffer(&ibd, &iinitData, &indexBuffer);
}

void Pyramid::move(D3DXVECTOR3 newPos)
{
	D3DXMatrixTranslation(&mWorld, newPos.x, newPos.y, newPos.z);
	pos = newPos;
}

void Pyramid::Draw()
{
	UINT stride = sizeof(Vertex);
	UINT offset = 0;
	d3dDevice->IASetVertexBuffers(0, 1, &vertexBuffer, &stride, &offset);
	d3dDevice->IASetIndexBuffer(indexBuffer, DXGI_FORMAT_R32_UINT, 0);
	d3dDevice->DrawIndexed(18, 0, 0);
}

D3DXMATRIX& Pyramid::getWorld()
{
	return mWorld;
}

int Pyramid::GetVertexCount()
{
	return (int)v.size();
}

std::vector<Vertex> Pyramid::CopyVertexArray()
{
	return v;
}