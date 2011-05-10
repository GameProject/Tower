#include "Cylinder.h"

Cylinder::Cylinder()
{
	d3dDevice = 0; 
	vertexBuffer = 0;
	indexBuffer = 0;
	nrOfVertices = 50;
}

Cylinder::~Cylinder()
{
	vertexBuffer->Release();
	indexBuffer->Release();
	vertexBuffer = 0;
	indexBuffer  = 0;
}

void Cylinder::init(ID3D10Device *device, float size, D3DXVECTOR3 pos)
{
	d3dDevice = device;
	D3DXMatrixIdentity(&mWorld);

	// Create vertex buffer
	v.resize(nrOfVertices);

	// Fill in the face vertex data.
	//top
	v[0] = Vertex( 0.0f, 1.0f,  0.0f, 0.0f, 1.0f, 0.0f, 0.5f, 0.5f, -1.0f, 0.0f, 0.0f);

	v[1] = Vertex(-0.5f, 1.0f,  1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.25f, -1.0f, 0.0f, 0.0f);
	v[2] = Vertex( 0.5f, 1.0f,  1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.75f, -1.0f, 0.0f, 0.0f);
	v[3] = Vertex( 1.0f, 1.0f,  0.5f, 0.0f, 1.0f, 0.0f, 0.25f, 1.0f, -1.0f, 0.0f, 0.0f);
	v[4] = Vertex( 1.0f, 1.0f, -0.5f, 0.0f, 1.0f, 0.0f, 0.75f, 1.0f, -1.0f, 0.0f, 0.0f);
	v[5] = Vertex( 0.5f, 1.0f, -1.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.75f, -1.0f, 0.0f, 0.0f);
	v[6] = Vertex(-0.5f, 1.0f, -1.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.25f, -1.0f, 0.0f, 0.0f);
	v[7] = Vertex(-1.0f, 1.0f, -0.5f, 0.0f, 1.0f, 0.0f, 0.75f, 0.0f, -1.0f, 0.0f, 0.0f);
	v[8] = Vertex(-1.0f, 1.0f,  0.5f, 0.0f, 1.0f, 0.0f, 0.25f, 0.0f, -1.0f, 0.0f, 0.0f);


	//bottom
	v[9] = Vertex( 0.0f, -1.0f,  0.0f, 0.0f, -1.0f, 0.0f, 0.5f, 0.5f, -1.0f, 0.0f, 0.0f);

	v[10] = Vertex(-0.5f, -1.0f,  1.0f, 0.0f, -1.0f, 0.0f, 0.0f, 0.25f, -1.0f, 0.0f, 0.0f);
	v[11] = Vertex( 0.5f, -1.0f,  1.0f, 0.0f, -1.0f, 0.0f, 0.0f, 0.75f, -1.0f, 0.0f, 0.0f);
	v[12] = Vertex( 1.0f, -1.0f,  0.5f, 0.0f, -1.0f, 0.0f, 0.25f, 1.0f, -1.0f, 0.0f, 0.0f);
	v[13] = Vertex( 1.0f, -1.0f, -0.5f, 0.0f, -1.0f, 0.0f, 0.75f, 1.0f, -1.0f, 0.0f, 0.0f);
	v[14] = Vertex( 0.5f, -1.0f, -1.0f, 0.0f, -1.0f, 0.0f, 1.0f, 0.75f, -1.0f, 0.0f, 0.0f);
	v[15] = Vertex(-0.5f, -1.0f, -1.0f, 0.0f, -1.0f, 0.0f, 1.0f, 0.25f, -1.0f, 0.0f, 0.0f);
	v[16] = Vertex(-1.0f, -1.0f, -0.5f, 0.0f, -1.0f, 0.0f, 0.75f, 0.0f, -1.0f, 0.0f, 0.0f);
	v[17] = Vertex(-1.0f, -1.0f,  0.5f, 0.0f, -1.0f, 0.0f, 0.25f, 0.0f, -1.0f, 0.0f, 0.0f);

 	//walls
 	v[18] = Vertex(-0.5f, -1.0f,  1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f);
 	v[19] = Vertex(-0.5f,  1.0f,  1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f);
 	v[20] = Vertex( 0.5f,  1.0f,  1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.5f, 0.0f, 1.0f, 0.0f);
 	v[21] = Vertex( 0.5f, -1.0f,  1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.5f, 0.0f, 1.0f, 0.0f);

	v[22] = Vertex( 0.5f, -1.0f,  1.0f, 1.0f, 0.0f, 1.0f, 1.0f, 0.5f, 0.0f, 1.0f, 0.0f);
	v[23] = Vertex( 0.5f,  1.0f,  1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.5f, 0.0f, 1.0f, 0.0f);
	v[24] = Vertex( 1.0f,  1.0f,  0.5f, 1.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f);
	v[25] = Vertex( 1.0f, -1.0f,  0.5f, 1.0f, 0.0f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f);

	v[26] = Vertex( 1.0f, -1.0f,  0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f);
	v[27] = Vertex( 1.0f,  1.0f,  0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f);
	v[28] = Vertex( 1.0f,  1.0f, -0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 1.5f, 0.0f, 1.0f, 0.0f);
	v[29] = Vertex( 1.0f, -1.0f, -0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 1.5f, 0.0f, 1.0f, 0.0f);

	v[30] = Vertex( 1.0f, -1.0f, -0.5f, 1.0f, 0.0f, -1.0f, 1.0f, 1.5f, 0.0f, 1.0f, 0.0f);
	v[31] = Vertex( 1.0f,  1.0f, -0.5f, 1.0f, 0.0f, -1.0f, 0.0f, 1.5f, 0.0f, 1.0f, 0.0f);
	v[32] = Vertex( 0.5f,  1.0f, -1.0f, 1.0f, 0.0f, -1.0f, 0.0f, 2.0f, 0.0f, 1.0f, 0.0f);
	v[33] = Vertex( 0.5f, -1.0f, -1.0f, 1.0f, 0.0f, -1.0f, 1.0f, 2.0f, 0.0f, 1.0f, 0.0f);

	v[34] = Vertex( 0.5f, -1.0f, -1.0f, 0.0f, -1.0f, 0.0f, 1.0f, 2.0f, 0.0f, 1.0f, 0.0f);
	v[35] = Vertex( 0.5f,  1.0f, -1.0f, 0.0f, -1.0f, 0.0f, 0.0f, 2.0f, 0.0f, 1.0f, 0.0f);
	v[36] = Vertex(-0.5f,  1.0f, -1.0f, 0.0f, -1.0f, 0.0f, 0.0f, 2.5f, 0.0f, 1.0f, 0.0f);
	v[37] = Vertex(-0.5f, -1.0f, -1.0f, 0.0f, -1.0f, 0.0f, 1.0f, 2.5f, 0.0f, 1.0f, 0.0f);

	v[38] = Vertex(-0.5f, -1.0f, -1.0f, -1.0f, 0.0f, -1.0f, 1.0f, 2.5f, 0.0f, 1.0f, 0.0f);
	v[39] = Vertex(-0.5f,  1.0f, -1.0f, -1.0f, 0.0f, -1.0f, 0.0f, 2.5f, 0.0f, 1.0f, 0.0f);
	v[40] = Vertex(-1.0f,  1.0f, -0.5f, -1.0f, 0.0f, -1.0f, 0.0f, 3.0f, 0.0f, 1.0f, 0.0f);
	v[41] = Vertex(-1.0f, -1.0f, -0.5f, -1.0f, 0.0f, -1.0f, 1.0f, 3.0f, 0.0f, 1.0f, 0.0f);

	v[42] = Vertex(-1.0f, -1.0f, -0.5f, -1.0f, 0.0f, 0.0f, 1.0f, 3.0f, 0.0f, 1.0f, 0.0f);
	v[43] = Vertex(-1.0f,  1.0f, -0.5f, -1.0f, 0.0f, 0.0f, 0.0f, 3.0f, 0.0f, 1.0f, 0.0f);
	v[44] = Vertex(-1.0f,  1.0f,  0.5f, -1.0f, 0.0f, 0.0f, 0.0f, 3.5f, 0.0f, 1.0f, 0.0f);
	v[45] = Vertex(-1.0f, -1.0f,  0.5f, -1.0f, 0.0f, 0.0f, 1.0f, 3.5f, 0.0f, 1.0f, 0.0f);

	v[46] = Vertex(-1.0f, -1.0f,  0.5f, -1.0f, 0.0f, 1.0f, 1.0f, 3.5f, 0.0f, 1.0f, 0.0f);
	v[47] = Vertex(-1.0f,  1.0f,  0.5f, -1.0f, 0.0f, 1.0f, 0.0f, 3.5f, 0.0f, 1.0f, 0.0f);
	v[48] = Vertex(-0.5f,  1.0f,  1.0f, -1.0f, 0.0f, 1.0f, 0.0f, 4.0f, 0.0f, 1.0f, 0.0f);
	v[49] = Vertex(-0.5f, -1.0f,  1.0f, -1.0f, 0.0f, 1.0f, 1.0f, 4.0f, 0.0f, 1.0f, 0.0f);

	// Scale the pyramid and move.
	for(DWORD i = 0; i < 50; ++i)
	{
		v[i].pos += pos;
		v[i].pos *= size;
		v[i].texC *= size;
		v[i].texC *= 2.0f;
	}

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
	DWORD i[96];
	// Fill in the face index data
	//top
	i[0]  = 1; i[1]  = 2; i[2]  = 0;
	i[3]  = 2; i[4]  = 3; i[5]  = 0;
	i[6]  = 3; i[7]  = 4; i[8]  = 0;
	i[9]  = 4; i[10] = 5; i[11] = 0;
	i[12] = 5; i[13] = 6; i[14] = 0;
	i[15] = 6; i[16] = 7; i[17] = 0;
	i[18] = 7; i[19] = 8; i[20] = 0;
	i[21] = 8; i[22] = 1; i[23] = 0;

	//bottom
	i[24] = 10; i[25] = 9; i[26] = 11;
	i[27] = 11; i[28] = 9; i[29] = 12;
	i[30] = 12; i[31] = 9; i[32] = 13;
	i[33] = 13; i[34] = 9; i[35] = 14;
	i[36] = 14; i[37] = 9; i[38] = 15;
	i[39] = 15; i[40] = 9; i[41] = 16;
	i[42] = 16; i[43] = 9; i[44] = 17;
	i[45] = 17; i[46] = 9; i[47] = 10;

	//walls
	i[48] = 20; i[49] = 19; i[50] = 18;
	i[51] = 20; i[52] = 18; i[53] = 21;

	i[54] = 24; i[55] = 23; i[56] = 22;
	i[57] = 24; i[58] = 22; i[59] = 25;

	i[60] = 28; i[61] = 27; i[62] = 26;
	i[63] = 28; i[64] = 26; i[65] = 29;

	i[66] = 32; i[67] = 31; i[68] = 30;
	i[69] = 32; i[70] = 30; i[71] = 33;

	i[72] = 36; i[73] = 35; i[74] = 34;
	i[75] = 36; i[76] = 34; i[77] = 37;

	i[78] = 40; i[79] = 39; i[80] = 38;
	i[81] = 40; i[82] = 38; i[83] = 41;

	i[84] = 44; i[85] = 43; i[86] = 42;
	i[87] = 44; i[88] = 42; i[89] = 45;

	i[90] = 48; i[91] = 47; i[92] = 46;
	i[93] = 48; i[94] = 46; i[95] = 49;



	D3D10_BUFFER_DESC ibd;
	ibd.Usage = D3D10_USAGE_IMMUTABLE;
	ibd.ByteWidth = sizeof(DWORD)*96;
	ibd.BindFlags = D3D10_BIND_INDEX_BUFFER;
	ibd.CPUAccessFlags = 0;
	ibd.MiscFlags = 0;
	D3D10_SUBRESOURCE_DATA iinitData;
	iinitData.pSysMem = i;
	d3dDevice->CreateBuffer(&ibd, &iinitData, &indexBuffer);
}

void Cylinder::Draw()
{
	UINT stride = sizeof(Vertex);
	UINT offset = 0;
	d3dDevice->IASetVertexBuffers(0, 1, &vertexBuffer, &stride, &offset);
	d3dDevice->IASetIndexBuffer(indexBuffer, DXGI_FORMAT_R32_UINT, 0);
	d3dDevice->DrawIndexed(96, 0, 0);
}

D3DXMATRIX& Cylinder::getWorld()
{
	return mWorld;
}