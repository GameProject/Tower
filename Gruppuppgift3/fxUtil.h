#ifndef FXUTIL_H
#define FXUTIL_H

#include "Light.h"
#include "Effects.h"
#include <vector>

class fxUtil
{
private:
	ID3D10Device* d3dDevice;
	ID3D10EffectTechnique*	mTech;
	D3DXMATRIX texMtx;
	D3D10_TECHNIQUE_DESC mTechDesc;

	ID3D10EffectMatrixVariable*	mfxWVPVar;
	ID3D10EffectMatrixVariable*	mfxWorldVar;
	ID3D10EffectMatrixVariable*	mfxTexMtxVar;

	ID3D10EffectScalarVariable*	mfxMeshTypeVar;
	ID3D10EffectScalarVariable*	mfxLightTypeVar;

	ID3D10EffectShaderResourceVariable* mfxDiffuseVar;
	ID3D10EffectShaderResourceVariable* mfxSpecMtxVar;
	ID3D10EffectShaderResourceVariable* mfxNormalMtxVar;

	ID3D10EffectShaderResourceVariable* mfxBlendMtxVar;
	ID3D10EffectShaderResourceVariable* mfxBlend1MtxVar;
	ID3D10EffectShaderResourceVariable* mfxBlend2MtxVar;
	ID3D10EffectShaderResourceVariable* mfxBlend3MtxVar;

	ID3D10EffectVariable*	mfxEyePosVar;
	ID3D10EffectVariable*	mfxLightVar;

	std::vector<ID3D10ShaderResourceView*> mTextureRVs;
public:
	fxUtil();
	~fxUtil();
	void init(ID3D10Device *device, ID3D10Effect* FX);
	void setRenderUtil(D3D10_TECHNIQUE_DESC &techDesc);
	D3D10_TECHNIQUE_DESC getTechDesc();
	void ApplyPassByIndex(UINT p);
	void setMfx(D3DXMATRIX WVP, D3DXMATRIX World, int n, int texture);
	void setSpecial(D3DXVECTOR3 eyePos, Light light, int l);
};
#endif