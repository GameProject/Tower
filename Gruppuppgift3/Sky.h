#ifndef SKYDOME_H
#define SKYDOME_H

#include <d3dx10.h>
#include <string>

class Sky
{
private:
	ID3D10Device* md3dDevice;
	ID3D10Buffer* mVB;
	ID3D10Buffer* mIB;

	ID3D10ShaderResourceView* mCubeMap;

	UINT mNumIndices;
 
	ID3D10EffectTechnique* mTech;
	ID3D10EffectMatrixVariable* mfxWVPVar;
	ID3D10EffectScalarVariable*	mfxLightTypeVar;
	ID3D10EffectShaderResourceVariable* mfxCubeMapVar;
public:
	Sky();
	~Sky();
	void init(ID3D10Device* device, std::wstring texRV, float radius);
	void draw(D3DXMATRIX v, D3DXMATRIX p, D3DXVECTOR3 pos, int l);
};

#endif // SKY_H