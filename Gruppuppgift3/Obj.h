#ifndef OBJ_H
#define OBJ_H

#include "Effects.h"
#include "Vertex.h"
#include <vector>
#include <string>
#include "fxUtil.h"
#include "Light.h"

class Obj
{
private:
	ID3D10EffectTechnique*	mTech;
	D3DXMATRIX texMtx;
	D3D10_TECHNIQUE_DESC mTechDesc;

	ID3D10EffectMatrixVariable*	mfxWVPVar;
	ID3D10EffectMatrixVariable*	mfxWorldVar;
	ID3D10EffectMatrixVariable*	mfxTexMtxVar;

	ID3D10EffectScalarVariable*	mfxMeshTypeVar;
	ID3D10EffectScalarVariable*	mfxLightTypeVar;

	ID3D10EffectShaderResourceVariable* mfxDiffuseVar;

	ID3D10EffectVariable*	mfxEyePosVar;
	ID3D10EffectVariable*	mfxLightVar;

	ID3D10ShaderResourceView* mTextureRV;

	D3DXMATRIX mWorld;
	D3DXMATRIX mRot;
	D3DXMATRIX mScale;
	D3DXVECTOR3 pos;
	float size;
	ID3D10Device* d3dDevice;
	ID3D10Buffer* vertexBuffer;
	ID3D10Buffer* indexBuffer;
	UINT mNumFaces, indexSize;
	std::vector<TerrainVertex> vertex;
public:
	Obj();
	void init(ID3D10Device* device, std::wstring filename);
	void setVB();
	void setIB();
	void move(D3DXVECTOR3 newPos);
	void scaleObj(float s);
	void rotate(float t);
	void Draw(D3DXMATRIX view, D3DXMATRIX proj);
	D3DXMATRIX& getWorld();
	D3DXVECTOR3 getPos();
	float getWidth();
	void setSpecial(D3DXVECTOR3 eyePos, Light light, int l);
	void Destroy();
};
#endif