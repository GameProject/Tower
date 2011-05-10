#ifndef BILLBOARD_H
#define BILLBOARD_H

#include <vector>
#include "Effects.h"
#include "Vertex.h"

class Billboard
{
private:
	UINT mNumTrees;

	ID3D10Device* md3dDevice;
	ID3D10Buffer* mVB;

	ID3D10EffectTechnique* mTech;
	ID3D10ShaderResourceView* mTreeMapArrayRV;
	ID3D10EffectMatrixVariable* mfxViewProjVar;
	ID3D10EffectVariable* mfxEyePosVar;

	ID3D10EffectShaderResourceVariable* mfxTreeMapArrayVar;
public:
	Billboard();
	~Billboard();
	void init(ID3D10Device* device, const D3DXVECTOR3 centers[], UINT numTrees, std::wstring texRV);
	void buildVB(const D3DXVECTOR3 centers[]);
	void draw(const D3DXVECTOR3& eyePosW, const D3DXMATRIX& viewProj);
};
#endif
