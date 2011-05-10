#include "Billboard.h"

Billboard::Billboard()
{
	md3dDevice = 0;
	mVB = 0;
}

Billboard::~Billboard()
{
	mVB->Release();
	mTreeMapArrayRV->Release();
	mVB = 0;
	mTreeMapArrayRV = 0;
}

void Billboard::init(ID3D10Device* device, const D3DXVECTOR3 centers[], UINT numTrees, std::wstring texRV)
{
	md3dDevice = device;
	mNumTrees = numTrees;

	D3DX10CreateShaderResourceViewFromFile(md3dDevice, texRV.c_str(), 0, 0, &mTreeMapArrayRV, 0 );

	buildVB(centers);

	mTech			   = Effects::BillboardFX->GetTechniqueByName("BillboardTech");
	mfxViewProjVar     = Effects::BillboardFX->GetVariableByName("gViewProj")->AsMatrix();
	mfxEyePosVar       = Effects::BillboardFX->GetVariableByName("gEyePosW");
	mfxTreeMapArrayVar = Effects::BillboardFX->GetVariableByName("gDiffuseMapArray")->AsShaderResource();
}

void Billboard::buildVB(const D3DXVECTOR3 centers[])
{
	std::vector<BillboardVertex> v;

	v.resize(mNumTrees);

	for(UINT i = 0; i < mNumTrees; ++i)
	{
		v[i].centerW  = centers[i];
		v[i].sizeW    = D3DXVECTOR2(24.0f, 24.0f);
	}

	D3D10_BUFFER_DESC vbd;
	vbd.Usage = D3D10_USAGE_IMMUTABLE;
	vbd.ByteWidth = sizeof(BillboardVertex) * mNumTrees;
	vbd.BindFlags = D3D10_BIND_VERTEX_BUFFER;
	vbd.CPUAccessFlags = 0;
	vbd.MiscFlags = 0;
	D3D10_SUBRESOURCE_DATA vinitData;
	vinitData.pSysMem = &v[0];
	md3dDevice->CreateBuffer(&vbd, &vinitData, &mVB);
}

void Billboard::draw(const D3DXVECTOR3& eyePosW, const D3DXMATRIX& viewProj)
{
	mfxEyePosVar->SetRawValue((void*)&eyePosW, 0, sizeof(D3DXVECTOR3));
	mfxViewProjVar->SetMatrix((float*)&viewProj);
	mfxTreeMapArrayVar->SetResource(mTreeMapArrayRV);

	md3dDevice->IASetPrimitiveTopology(D3D10_PRIMITIVE_TOPOLOGY_POINTLIST);
	md3dDevice->IASetInputLayout(Effects::PosSize);

	D3D10_TECHNIQUE_DESC techDesc;
    mTech->GetDesc( &techDesc );

    for(UINT i = 0; i < techDesc.Passes; ++i)
    {
        ID3D10EffectPass* pass = mTech->GetPassByIndex(i);
		pass->Apply(0);

		UINT stride = sizeof(BillboardVertex);
		UINT offset = 0;
		md3dDevice->IASetVertexBuffers(0, 1, &mVB, &stride, &offset);
		md3dDevice->Draw(mNumTrees, 0);
	}
}