#include "fxUtil.h"
#include "Effects.h"

fxUtil::fxUtil()
{
	d3dDevice = 0;
}

fxUtil::~fxUtil()
{
// 	mCubeDiffuseRV->Release();
// 	mCubeSpecMapRV->Release();
// 	mCubeNormalMapRV->Release();
// 	mTerrainDiffuseRV->Release();
// 	mTerrainNormalMapRV->Release();
// 	mCubeDiffuseRV = 0;
// 	mCubeSpecMapRV = 0;
// 	mCubeNormalMapRV = 0;
// 	mTerrainDiffuseRV = 0;
// 	mTerrainNormalMapRV = 0;
}

void fxUtil::init(ID3D10Device* device, ID3D10Effect* FX)
{
	D3DXMatrixIdentity(&texMtx);

	d3dDevice = device;
	mTech			 = FX->GetTechniqueByName("Tech");
	mfxWorldVar		 = FX->GetVariableByName("gWorld")->AsMatrix();
	mfxWVPVar        = FX->GetVariableByName("gWVP")->AsMatrix();
	mfxMeshTypeVar	 = FX->GetVariableByName("gType")->AsScalar();
	mfxLightTypeVar	 = FX->GetVariableByName("gLightType")->AsScalar();
	mfxTexMtxVar     = FX->GetVariableByName("gTexMtx")->AsMatrix();
	mfxDiffuseVar	 = FX->GetVariableByName("gDiffuseMap")->AsShaderResource();
	mfxSpecMtxVar    = FX->GetVariableByName("gSpecMap")->AsShaderResource();
	mfxNormalMtxVar  = FX->GetVariableByName("gNormalMap")->AsShaderResource();
	mfxLightVar      = FX->GetVariableByName("gLight");
	mfxEyePosVar     = FX->GetVariableByName("gEyePos");

	mfxBlendMtxVar   = FX->GetVariableByName("gBlendMap")->AsShaderResource();
	mfxBlend1MtxVar  = FX->GetVariableByName("gBlend1Map")->AsShaderResource();
	mfxBlend2MtxVar  = FX->GetVariableByName("gBlend2Map")->AsShaderResource();
	mfxBlend3MtxVar  = FX->GetVariableByName("gBlend3Map")->AsShaderResource();

	mTextureRVs.resize(16);
	//Cube
	D3DX10CreateShaderResourceViewFromFile(d3dDevice, 
		L"Textures/Rock/Stonewall.jpg", 0, 0, &mTextureRVs[0], 0 );
	D3DX10CreateShaderResourceViewFromFile(d3dDevice, 
		L"Textures/Rock/Stonewall_Specular.jpg", 0, 0, &mTextureRVs[1], 0 );
	D3DX10CreateShaderResourceViewFromFile(d3dDevice,
		L"Textures/Rock/Stonewall_Normal.jpg", 0, 0, &mTextureRVs[2], 0 );
	//--------------------------------------------

	//Pyramid
	D3DX10CreateShaderResourceViewFromFile(d3dDevice, 
		L"Textures/Rock/Stone_Diffuse.jpg", 0, 0, &mTextureRVs[3], 0 );
	D3DX10CreateShaderResourceViewFromFile(d3dDevice, 
		L"Textures/Rock/Stone_Specular.jpg", 0, 0, &mTextureRVs[4], 0 );
	D3DX10CreateShaderResourceViewFromFile(d3dDevice,
		L"Textures/Rock/Stone_Normal.jpg", 0, 0, &mTextureRVs[5], 0 );
	//--------------------------------------------

	//Cylinder
	D3DX10CreateShaderResourceViewFromFile(d3dDevice, 
		L"Textures/Metal/MetalSheet.jpg", 0, 0, &mTextureRVs[6], 0 );
	D3DX10CreateShaderResourceViewFromFile(d3dDevice, 
		L"Textures/Metal/MetalSheet_Specular.jpg", 0, 0, &mTextureRVs[7], 0 );
	D3DX10CreateShaderResourceViewFromFile(d3dDevice,
		L"Textures/Metal/MetalSheet_Normal.jpg", 0, 0, &mTextureRVs[8], 0 );
	//--------------------------------------------

	//Terrain
	D3DX10CreateShaderResourceViewFromFile(d3dDevice, 
		L"Textures/Terrain/Sand.jpg", 0, 0, &mTextureRVs[9], 0 ); 
	D3DX10CreateShaderResourceViewFromFile(d3dDevice,
		L"Textures/Terrain/Normal.jpg", 0, 0, &mTextureRVs[10], 0 );
	D3DX10CreateShaderResourceViewFromFile(d3dDevice, 
		L"Textures/Terrain/BlendMap.jpg", 0, 0, &mTextureRVs[11], 0 );
	D3DX10CreateShaderResourceViewFromFile(d3dDevice, 
		L"Textures/Terrain/Grass.jpg", 0, 0, &mTextureRVs[12], 0 );
	D3DX10CreateShaderResourceViewFromFile(d3dDevice,
		L"Textures/Terrain/Rock.jpg", 0, 0, &mTextureRVs[13], 0 );
	D3DX10CreateShaderResourceViewFromFile(d3dDevice,
		L"Textures/Terrain/Rock2.jpg", 0, 0, &mTextureRVs[14], 0 );
	//--------------------------------------------

	//Obj
	D3DX10CreateShaderResourceViewFromFile(d3dDevice, 
		L"Objects/bth_logo_obj/bthcolor.dds", 0, 0, &mTextureRVs[15], 0 );
}

void fxUtil::setRenderUtil(D3D10_TECHNIQUE_DESC &techDesc)
{
	d3dDevice->IASetInputLayout(Effects::PosTangentNormalTex);
	d3dDevice->IASetPrimitiveTopology(D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	mTech->GetDesc( &techDesc );
	mTechDesc = techDesc;
}

D3D10_TECHNIQUE_DESC fxUtil::getTechDesc()
{
	return mTechDesc;
}

void fxUtil::ApplyPassByIndex(UINT p)
{
	mTech->GetPassByIndex( p )->Apply(0);
}

void fxUtil::setMfx(D3DXMATRIX WVP, D3DXMATRIX World, int n, int texture)
{
	mfxWVPVar->SetMatrix((float*)&WVP);
	mfxWorldVar->SetMatrix((float*)&World);
	
	mfxMeshTypeVar->SetInt(n);
	mfxDiffuseVar->SetResource(mTextureRVs[texture]);
	if (n != 4)
	{
		switch (n)
		{
		case 0:
			mfxNormalMtxVar->SetResource(mTextureRVs[texture+1]);
			mfxBlendMtxVar->SetResource(mTextureRVs[texture+2]);
			mfxBlend1MtxVar->SetResource(mTextureRVs[texture+3]);
			mfxBlend2MtxVar->SetResource(mTextureRVs[texture+4]);
			mfxBlend3MtxVar->SetResource(mTextureRVs[texture+5]);
			break;
		default:
			mfxSpecMtxVar->SetResource(mTextureRVs[texture+1]);
			mfxNormalMtxVar->SetResource(mTextureRVs[texture+2]);
			break;
		}
	}
}

void fxUtil::setSpecial(D3DXVECTOR3 eyePos, Light light, int l)
{
	mfxTexMtxVar->SetMatrix((float*)&texMtx);
	mfxEyePosVar->SetRawValue(&eyePos, 0, sizeof(D3DXVECTOR3));
	mfxLightVar->SetRawValue(&light, 0, sizeof(Light));
	mfxLightTypeVar->SetInt(l);
}