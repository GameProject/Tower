#include "Sky.h"
#include "Effects.h"
 
Sky::Sky()
{
	md3dDevice = 0;
	mVB = 0;
	mIB = 0;
	mCubeMap = 0;
	mNumIndices = 0;
}

Sky::~Sky()
{
	mVB->Release();
	mIB->Release();
	mVB = 0;
	mIB = 0;
}

void Sky::init(ID3D10Device* device, std::wstring texRV, float radius)
{
	md3dDevice = device;

	D3DX10CreateShaderResourceViewFromFile(md3dDevice, texRV.c_str(), 0, 0, &mCubeMap, 0 );

	mTech			 = Effects::SkyFX->GetTechniqueByName("SkyTech");
	mfxWVPVar		 = Effects::SkyFX->GetVariableByName("gWVP")->AsMatrix();
	mfxCubeMapVar	 = Effects::SkyFX->GetVariableByName("gCubeMap")->AsShaderResource();
	mfxLightTypeVar	 = Effects::SkyFX->GetVariableByName("gLightType")->AsScalar();

	D3DXVECTOR3 skyVerts[24];

	skyVerts[0] = D3DXVECTOR3(-1, -1, -1);
	skyVerts[1] = D3DXVECTOR3(-1,  1, -1);
	skyVerts[2] = D3DXVECTOR3( 1,  1, -1);
	skyVerts[3] = D3DXVECTOR3( 1, -1, -1);

	// Fill in the back face vertex data.
	skyVerts[4] = D3DXVECTOR3(-1, -1, 1);
	skyVerts[5] = D3DXVECTOR3( 1, -1, 1);
	skyVerts[6] = D3DXVECTOR3( 1,  1, 1);
	skyVerts[7] = D3DXVECTOR3(-1,  1, 1);

	// Fill in the top face vertex data.
	skyVerts[8]  = D3DXVECTOR3(-1, 1, -1);
	skyVerts[9]  = D3DXVECTOR3(-1, 1,  1);
	skyVerts[10] = D3DXVECTOR3( 1, 1,  1);
	skyVerts[11] = D3DXVECTOR3( 1, 1, -1);

	// Fill in the bottom face vertex data.
	skyVerts[12] = D3DXVECTOR3(-1, -1, -1);
	skyVerts[13] = D3DXVECTOR3( 1, -1, -1);
	skyVerts[14] = D3DXVECTOR3( 1, -1,  1);
	skyVerts[15] = D3DXVECTOR3(-1, -1,  1);

	// Fill in the left face vertex data.
	skyVerts[16] = D3DXVECTOR3(-1, -1,  1);
	skyVerts[17] = D3DXVECTOR3(-1,  1,  1);
	skyVerts[18] = D3DXVECTOR3(-1,  1, -1);
	skyVerts[19] = D3DXVECTOR3(-1, -1, -1);

	// Fill in the right face vertex data.
	skyVerts[20] = D3DXVECTOR3( 1, -1, -1);
	skyVerts[21] = D3DXVECTOR3( 1,  1, -1);
	skyVerts[22] = D3DXVECTOR3( 1,  1,  1);
	skyVerts[23] = D3DXVECTOR3( 1, -1,  1);

	D3D10_BUFFER_DESC vbd;
    vbd.Usage = D3D10_USAGE_IMMUTABLE;
	vbd.ByteWidth = sizeof(D3DXVECTOR3) * 24;
    vbd.BindFlags = D3D10_BIND_VERTEX_BUFFER;
    vbd.CPUAccessFlags = 0;
    vbd.MiscFlags = 0;
    D3D10_SUBRESOURCE_DATA vinitData;
    vinitData.pSysMem = &skyVerts[0];
    md3dDevice->CreateBuffer(&vbd, &vinitData, &mVB);

	DWORD i[36];

	// Fill in the front face index data
	i[0] = 3; i[1] = 2; i[2] = 0;
	i[3] = 2; i[4] = 1; i[5] = 0;

	// Fill in the back face index data
	i[6] = 7; i[7]  = 6; i[8]  = 4;
	i[9] = 6; i[10] = 5; i[11] = 4;

	// Fill in the top face index data
	i[12] = 11; i[13] = 10; i[14] = 8;
	i[15] = 10; i[16] = 9;  i[17] = 8;

	// Fill in the bottom face index data
	i[18] = 15; i[19] = 14; i[20] = 12;
	i[21] = 14; i[22] = 13; i[23] = 12;

	// Fill in the left face index data
	i[24] = 19; i[25] = 18; i[26] = 16;
	i[27] = 18; i[28] = 17; i[29] = 16;

	// Fill in the right face index data
	i[30] = 23; i[31] = 22; i[32] = 20;
	i[33] = 22; i[34] = 21; i[35] = 20;
	mNumIndices = (UINT)36;

	D3D10_BUFFER_DESC ibd;
    ibd.Usage = D3D10_USAGE_IMMUTABLE;
	ibd.ByteWidth = sizeof(DWORD) * mNumIndices;
    ibd.BindFlags = D3D10_BIND_INDEX_BUFFER;
    ibd.CPUAccessFlags = 0;
    ibd.MiscFlags = 0;
    D3D10_SUBRESOURCE_DATA iinitData;
    iinitData.pSysMem = i;
    md3dDevice->CreateBuffer(&ibd, &iinitData, &mIB);
}

void Sky::draw(D3DXMATRIX v, D3DXMATRIX p, D3DXVECTOR3 pos, int l)
{
	D3DXVECTOR3 eyePos = pos;

	// center Sky about eye in world space
	D3DXMATRIX W;
	D3DXMatrixTranslation(&W, eyePos.x, eyePos.y, eyePos.z);

	D3DXMATRIX V = v;
	D3DXMATRIX P = p;

	D3DXMATRIX WVP = W*V*P;

	mfxWVPVar->SetMatrix((float*)WVP);
	mfxCubeMapVar->SetResource(mCubeMap);
	mfxLightTypeVar->SetInt(l);

	md3dDevice->IASetInputLayout(Effects::Pos);
	md3dDevice->IASetPrimitiveTopology(D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	UINT stride = sizeof(D3DXVECTOR3);
    UINT offset = 0;
    md3dDevice->IASetVertexBuffers(0, 1, &mVB, &stride, &offset);
	md3dDevice->IASetIndexBuffer(mIB, DXGI_FORMAT_R32_UINT, 0);
	
	D3D10_TECHNIQUE_DESC techDesc;
    mTech->GetDesc( &techDesc );

    for(UINT p = 0; p < techDesc.Passes; ++p)
    {
        ID3D10EffectPass* pass = mTech->GetPassByIndex(p);
		pass->Apply(0);
		md3dDevice->DrawIndexed(mNumIndices, 0, 0);
	}
}
 