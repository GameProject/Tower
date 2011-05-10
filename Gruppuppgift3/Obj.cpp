#include "Obj.h"
#include <fstream>
#include <istream>

using namespace std;

Obj::Obj()
{
	d3dDevice = 0; 
	vertexBuffer = 0;
	indexBuffer = 0;

	mNumFaces	 = 0;
	indexSize	 = 0;
}

void Obj::init(ID3D10Device *device, std::wstring filename)
{
	d3dDevice = device;

	D3DXMatrixIdentity(&mWorld);
	D3DXMatrixIdentity(&mRot);
	D3DXMatrixIdentity(&mScale);
	D3DXMatrixIdentity(&texMtx);

	ifstream fin(filename.c_str());

	string ignore;
	char slash;
	int inten;

	float x, y, z;

	
	vector<D3DXVECTOR2> texC;
	vector<D3DXVECTOR3> normal;
	vector<D3DXVECTOR3> pos;

	TerrainVertex obj;

	if (fin)
	{
		for (int i=0; i<15; ++i)
			fin >> ignore;

		float s = 0;
		while(ignore == "v")
		{
			fin >> x >> y >> z;
			pos.push_back(D3DXVECTOR3(x, y, z));
			fin >> ignore;
		}
		while(ignore == "vt")
		{
			fin >> x >> y;
			y *= -1.0f;
			texC.push_back(D3DXVECTOR2(x, y));
			fin >> ignore;
		}
		while(ignore == "vn")
		{
			fin >> x >> y >> z;
			normal.push_back(D3DXVECTOR3(x, y, z));
			fin >> ignore;
		}

		fin >> ignore;
		int isS = 3;
		if (ignore == "off")
			isS = 4;

		for(int i=0; i<5; ++i)
			fin >> ignore;		
		while (!fin.eof())
		{
			for (int k=0; k<2; ++k)
			{
				for (int i = 0; i<isS; ++i)
				{
					fin >> x >> slash >> y >> slash >> z;

					obj.pos = pos.at((int)x-1);
					obj.texC = texC.at((int)y-1);
					obj.normal = normal.at((int)z-1);

					vertex.push_back(obj);
				}
				s += isS;
				fin >> ignore;
				++mNumFaces;
			}
			if (isS == 3)
				fin >> inten >> ignore;
		}
		indexSize = (UINT)s;
	}
	setVB();
	setIB();

	mTech			 = Effects::MeshFX->GetTechniqueByName("Tech");
	mfxWorldVar		 = Effects::MeshFX->GetVariableByName("gWorld")->AsMatrix();
	mfxWVPVar        = Effects::MeshFX->GetVariableByName("gWVP")->AsMatrix();
	mfxMeshTypeVar	 = Effects::MeshFX->GetVariableByName("gType")->AsScalar();
	mfxLightTypeVar	 = Effects::MeshFX->GetVariableByName("gLightType")->AsScalar();
	mfxTexMtxVar     = Effects::MeshFX->GetVariableByName("gTexMtx")->AsMatrix();
	mfxDiffuseVar	 = Effects::MeshFX->GetVariableByName("gDiffuseMap")->AsShaderResource();
	mfxLightVar      = Effects::MeshFX->GetVariableByName("gLight");
	mfxEyePosVar     = Effects::MeshFX->GetVariableByName("gEyePos");

	D3DX10CreateShaderResourceViewFromFile(d3dDevice, 
		L"Objects/bth_logo_obj/bthcolor.dds", 0, 0, &mTextureRV, 0 );
}

void Obj::setVB()
{
	D3D10_BUFFER_DESC vbd;
	vbd.Usage = D3D10_USAGE_IMMUTABLE;
	vbd.ByteWidth = sizeof(TerrainVertex) * (UINT)vertex.size();
	vbd.BindFlags = D3D10_BIND_VERTEX_BUFFER;
	vbd.CPUAccessFlags = 0;
	vbd.MiscFlags = 0;
	D3D10_SUBRESOURCE_DATA vinitData;
	vinitData.pSysMem = &vertex[0];
	d3dDevice->CreateBuffer(&vbd, &vinitData, &vertexBuffer);
}

void Obj::setIB()
{
	DWORD* i = new DWORD[indexSize];

	for (int k=0; k<(int)indexSize; ++k)
		i[k] = (DWORD)k;
	
	D3D10_BUFFER_DESC ibd;
	ibd.Usage = D3D10_USAGE_IMMUTABLE;
	ibd.ByteWidth = sizeof(DWORD) * mNumFaces*3;
	ibd.BindFlags = D3D10_BIND_INDEX_BUFFER;
	ibd.CPUAccessFlags = 0;
	ibd.MiscFlags = 0;
	D3D10_SUBRESOURCE_DATA iinitData;
	iinitData.pSysMem = i;
	d3dDevice->CreateBuffer(&ibd, &iinitData, &indexBuffer);
}

void Obj::move(D3DXVECTOR3 newPos)
{
	D3DXMATRIX T;
	D3DXMatrixTranslation(&T, pos.x, pos.y, pos.z);

	D3DXMatrixScaling(&mScale, -size, -size, -size);

	D3DXMatrixTranslation(&mWorld, newPos.x, newPos.y, newPos.z);
	scaleObj(size);
	pos = newPos;
}

void Obj::scaleObj(float s)
{
	D3DXMATRIX T;
	D3DXMatrixTranslation(&T, pos.x, pos.y, pos.z);

	D3DXMatrixScaling(&mScale, s, s, s);
	size = s;
	mWorld = mRot*mScale*T;
}

void Obj::rotate(float t)
{
	D3DXMATRIX T;
	D3DXMatrixTranslation(&T, pos.x, pos.y, pos.z);
	D3DXMATRIX Ry;
	D3DXMatrixRotationY(&Ry, t*0.5f);
	mRot = Ry;
	mWorld = mRot*mScale*T;
}

void Obj::Draw(D3DXMATRIX view, D3DXMATRIX proj)
{
	d3dDevice->IASetPrimitiveTopology(D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	d3dDevice->IASetInputLayout(Effects::PosTangentNormalTex);

	UINT stride = sizeof(TerrainVertex);
	UINT offset = 0;
	d3dDevice->IASetVertexBuffers(0, 1, &vertexBuffer, &stride, &offset);
	d3dDevice->IASetIndexBuffer(indexBuffer, DXGI_FORMAT_R32_UINT, 0);

	D3DXMATRIX WVP = mWorld * view * proj;

	mfxWVPVar->SetMatrix((float*)&WVP);
	mfxWorldVar->SetMatrix((float*)&mWorld);

	mfxMeshTypeVar->SetInt(4);
	mfxDiffuseVar->SetResource(mTextureRV);
	
	D3D10_TECHNIQUE_DESC techDesc;
	mTech->GetDesc( &techDesc );

	for(UINT i = 0; i < techDesc.Passes; ++i)
	{
		ID3D10EffectPass* pass = mTech->GetPassByIndex(i);
		pass->Apply(0);

		d3dDevice->DrawIndexed(indexSize, 0, 0);
	}
}

D3DXMATRIX& Obj::getWorld()
{
	return mWorld;
}

D3DXVECTOR3 Obj::getPos()
{
	return pos;
}

float Obj::getWidth()
{
	float minX = NULL;
	float maxX = NULL;
	float minZ = NULL;
	float maxZ = NULL;
	for (int i=0; i<(int)vertex.size(); ++i)
	{
		if (minX == NULL)
			minX = vertex.at(i).pos.x;
		else if (minX > vertex.at(i).pos.x)
			minX = vertex.at(i).pos.x;

		if (maxX == NULL)
			maxX = vertex.at(i).pos.x;
		else if (maxX < vertex.at(i).pos.x)
			maxX = vertex.at(i).pos.x;

		if (minZ == NULL)
			minZ = vertex.at(i).pos.z;
		else if (minZ > vertex.at(i).pos.z)
			minZ = vertex.at(i).pos.z;

		if (maxZ == NULL)
			maxZ = vertex.at(i).pos.z;
		else if (maxZ < vertex.at(i).pos.z)
			maxZ = vertex.at(i).pos.z;
	}

	if ((minX + maxX)*0.5f > (minZ + maxZ)*0.5f)
		return (minX + maxX)*0.5f;
	return (minZ + maxZ)*0.5f;
}

void Obj::setSpecial(D3DXVECTOR3 eyePos, Light light, int l)
{
	mfxTexMtxVar->SetMatrix((float*)&texMtx);
	mfxEyePosVar->SetRawValue(&eyePos, 0, sizeof(D3DXVECTOR3));
	mfxLightVar->SetRawValue(&light, 0, sizeof(Light));
	mfxLightTypeVar->SetInt(l);
}

void Obj::Destroy()
{
	d3dDevice->Release();
	vertexBuffer->Release();
	indexBuffer->Release();
	d3dDevice = 0;
	vertexBuffer = 0;
	indexBuffer = 0;
}