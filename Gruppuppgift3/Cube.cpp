#include "Cube.h"
#include "Camera.h"

Cube::Cube()
{
	this->cId = -1;
	d3dDevice = 0;
	vertexBuffer = 0;
	indexBuffer  = 0;
	nrOfVertices = 24;
	nrOfFaces	 = 12;
	srand(time(NULL));
}

void Cube::release()
{
	vertexBuffer->Release();
	indexBuffer->Release();
	vertexBuffer = 0;
	indexBuffer  = 0;
}

void Cube::init(int id, ID3D10Device *device, D3DXVECTOR3 size, D3DXVECTOR3 pos, float timer)
{
	d3dDevice = device;	
	D3DXMatrixIdentity(&mWorld);
	D3DXMatrixIdentity(&mRot);
	D3DXMatrixIdentity(&mScale);

	this->cId = id;
	this->sTimer = timer;
	this->sStart = 0;
	offsetPos =  D3DXVECTOR3((float)(rand() % 40 + (-20)),0.0f,(float)(rand() % 40 + (-20)));
	this->speed = 20.0f;
	


	// Create vertex buffer
	v.resize(nrOfVertices);

	// Fill in the front face vertex data.
	v[0] = Vertex(-size.x, -size.y, -size.z, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f);
	v[1] = Vertex(-size.x,  size.y, -size.z, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f);
	v[2] = Vertex( size.x,  size.y, -size.z, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f);
	v[3] = Vertex( size.x, -size.y, -size.z, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f);

	// Fill in the back face vertex data.
	v[4] = Vertex(-size.x, -size.y, size.z, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f);
	v[5] = Vertex( size.x, -size.y, size.z, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f);
	v[6] = Vertex( size.x,  size.y, size.z, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f);
	v[7] = Vertex(-size.x,  size.y, size.z, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f);

	// Fill in the top face vertex data.
	v[8]  = Vertex(-size.x, size.y, -size.z, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f);
	v[9]  = Vertex(-size.x, size.y,  size.z, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f);
	v[10] = Vertex( size.x, size.y,  size.z, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f);
	v[11] = Vertex( size.x, size.y, -size.z, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f);

	// Fill in the bottom face vertex data.
	v[12] = Vertex(-size.x, -size.y, -size.z, 0.0f, -1.0f, 0.0f, 1.0f, 1.0f, -1.0f, 0.0f, 0.0f);
	v[13] = Vertex( size.x, -size.y, -size.z, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f, -1.0f, 0.0f, 0.0f);
	v[14] = Vertex( size.x, -size.y,  size.z, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f);
	v[15] = Vertex(-size.x, -size.y,  size.z, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f, -1.0f, 0.0f, 0.0f);

	// Fill in the left face vertex data.
	v[16] = Vertex(-size.x, -size.y,  size.z, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, -1.0f);
	v[17] = Vertex(-size.x,  size.y,  size.z, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f);
	v[18] = Vertex(-size.x,  size.y, -size.z, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, -1.0f);
	v[19] = Vertex(-size.x, -size.y, -size.z, -1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, -1.0f);

	// Fill in the right face vertex data.
	v[20] = Vertex( size.x, -size.y, -size.z, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f);
	v[21] = Vertex( size.x,  size.y, -size.z, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f);
	v[22] = Vertex( size.x,  size.y,  size.z, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f);
	v[23] = Vertex( size.x, -size.y,  size.z, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f);

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
	DWORD i[36];
	wp.LoadFromFile("test");
	// Fill in the front face index data
	i[0] = 0; i[1] = 1; i[2] = 2;
	i[3] = 0; i[4] = 2; i[5] = 3;

	// Fill in the back face index data
	i[6] = 4; i[7]  = 5; i[8]  = 6;
	i[9] = 4; i[10] = 6; i[11] = 7;

	// Fill in the top face index data
	i[12] = 8; i[13] =  9; i[14] = 10;
	i[15] = 8; i[16] = 10; i[17] = 11;

	// Fill in the bottom face index data
	i[18] = 12; i[19] = 13; i[20] = 14;
	i[21] = 12; i[22] = 14; i[23] = 15;

	// Fill in the left face index data
	i[24] = 16; i[25] = 17; i[26] = 18;
	i[27] = 16; i[28] = 18; i[29] = 19;

	// Fill in the right face index data
	i[30] = 20; i[31] = 21; i[32] = 22;
	i[33] = 20; i[34] = 22; i[35] = 23;

	D3D10_BUFFER_DESC ibd;
	ibd.Usage = D3D10_USAGE_IMMUTABLE;
	ibd.ByteWidth = sizeof(DWORD) * nrOfFaces*3;
	ibd.BindFlags = D3D10_BIND_INDEX_BUFFER;
	ibd.CPUAccessFlags = 0;
	ibd.MiscFlags = 0;
	D3D10_SUBRESOURCE_DATA iinitData;
	iinitData.pSysMem = i;
	d3dDevice->CreateBuffer(&ibd, &iinitData, &indexBuffer);

	this->eI.eID = this->cId;
	this->eI.ePos = this->pos;
	this->eI.eRadius = size.x;//Tillf�llig
	this->hasStarted = false;
	this->hasGold = false;
}


void Cube::update(float dt, Terrain* hM)
{

	if(this->wp.getCurrent() == 0 && !hasStarted)
	{
		sStart = GetTime().getGameTime();
		this->hasStarted = true;
	}

	if(this->wp.getCurrent() == (this->wp.getwP().size() - 1))
	{
		if(GetTime().getGameTime() >= goldTimer+7.0f)
		{
			this->hasGold = true;
			sStart = GetTime().getGameTime();
			this->wp.reset();
		}
	}
	else
	{
		float temp = GetTime().getGameTime();
		if((this->sStart+sTimer) <= temp && !this->hasGold)
		{
			this->WaypointMove(GetTime().getDeltaTime(), hM);
		}
		else if(this->hasGold)
		{
			this->WaypointMove(GetTime().getDeltaTime(), hM);
		}
		this->goldTimer = GetTime().getGameTime();
	}

	this->eI.ePos = this->pos;
}

void Cube::rotate(float t)
{
	D3DXMATRIX T;
	D3DXMatrixTranslation(&T, pos.x, pos.y, pos.z);
	D3DXMATRIX Rx;
	D3DXMATRIX Rz;
	D3DXMatrixRotationX(&Rx, t*0.5f);
	D3DXMatrixRotationZ(&Rz, t*0.5f);
	mRot = Rx*Rz;
	mWorld = (mRot*mScale)*T;
}

void Cube::move(D3DXVECTOR3 newPos)
{
	D3DXMatrixTranslation(&mWorld, newPos.x, newPos.y, newPos.z);
	pos = newPos;
}

void Cube::scaleCube(float s)
{
	D3DXMATRIX T;
	D3DXMatrixTranslation(&T, pos.x, pos.y, pos.z);

	D3DXMatrixScaling(&mScale, s, s, s);

	mWorld = (mScale*mRot)*T;
}

void Cube::Draw()
{
	UINT stride = sizeof(Vertex);
	UINT offset = 0;
	d3dDevice->IASetVertexBuffers(0, 1, &vertexBuffer, &stride, &offset);
	d3dDevice->IASetIndexBuffer(indexBuffer, DXGI_FORMAT_R32_UINT, 0);
	d3dDevice->DrawIndexed(36, 0, 0);
}

void Cube::setUV(float d, bool alongX)
{
	if (alongX)
		for (int i=0; i<(int)v.size(); ++i)
			v.at(i).texC.x -= d;
	else
		for (int i=0; i<(int)v.size(); ++i)
			v.at(i).texC.y -= d;
	D3D10_BUFFER_DESC vbd;
	vbd.Usage = D3D10_USAGE_IMMUTABLE;
	vbd.ByteWidth = sizeof(Vertex) * nrOfVertices;
	vbd.BindFlags = D3D10_BIND_VERTEX_BUFFER;
	vbd.CPUAccessFlags = 0;
	vbd.MiscFlags = 0;
	D3D10_SUBRESOURCE_DATA vinitData;
	vinitData.pSysMem = &v[0];
	d3dDevice->CreateBuffer(&vbd, &vinitData, &vertexBuffer);
}

D3DXMATRIX& Cube::getWorld()
{
	return mWorld;
}

void Cube::WaypointMove(float dt, Terrain* hM)
{

	D3DXVECTOR3 tempV;
	tempV = wp.getMove(dt,this->speed);
	//tempV += this->offsetPos;
	this->move(D3DXVECTOR3(tempV.x,hM->getHeight(tempV.x,tempV.z)+1,tempV.z));

}
float Cube::getTimeToStart()
{
	return this->sTimer;
}
Waypoint Cube::getWp()
{
	return this->wp;
}
enemyInfo Cube::getEI()
{
	return this->eI;
}

