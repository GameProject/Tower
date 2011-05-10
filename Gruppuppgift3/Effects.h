#ifndef EFFECTS_H
#define EFFECTS_H

#include <d3dx10.h>

namespace Effects
{
	extern ID3D10InputLayout* PosTangentNormalTex;
	extern ID3D10InputLayout* Particle;
	extern ID3D10InputLayout* Pos;
	extern ID3D10InputLayout* PosSize;
	extern ID3D10InputLayout* PosColor;

	extern ID3D10Effect* MeshFX;
	extern ID3D10Effect* FireFX;
	extern ID3D10Effect* RainFX;
	extern ID3D10Effect* SkyFX;
	extern ID3D10Effect* BillboardFX;
	extern ID3D10Effect* AABBFX;

	void InitAll(ID3D10Device* device);
	void DestroyAll();
};

#endif // INPUTLAYOUTS_H