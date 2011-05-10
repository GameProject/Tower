#include "Effects.h"

ID3D10InputLayout* Effects::PosTangentNormalTex = 0;
ID3D10InputLayout* Effects::Particle			= 0;
ID3D10InputLayout* Effects::Pos					= 0;
ID3D10InputLayout* Effects::PosSize				= 0;
ID3D10InputLayout* Effects::PosColor			= 0;

ID3D10Effect* Effects::MeshFX		= 0;
ID3D10Effect* Effects::FireFX		= 0;
ID3D10Effect* Effects::RainFX		= 0;
ID3D10Effect* Effects::SkyFX		= 0;
ID3D10Effect* Effects::BillboardFX	= 0;
ID3D10Effect* Effects::AABBFX		= 0;

void Effects::InitAll(ID3D10Device* device)
{
	D3D10_PASS_DESC PassDesc;

	DWORD shaderFlags = D3D10_SHADER_ENABLE_STRICTNESS;
#if defined( DEBUG ) || defined( _DEBUG )
	shaderFlags |= D3D10_SHADER_DEBUG;
	shaderFlags |= D3D10_SHADER_SKIP_OPTIMIZATION;
#endif

	ID3D10Blob* compilationErrors = 0;


	//
	//	Mesh
	//
	D3D10_INPUT_ELEMENT_DESC vertexDesc[] =
	{
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0,  D3D10_INPUT_PER_VERTEX_DATA, 0},
		{"NORMAL",   0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D10_INPUT_PER_VERTEX_DATA, 0},
		{"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT,    0, 24, D3D10_INPUT_PER_VERTEX_DATA, 0},
		{"TANGENT",  0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 36, D3D10_INPUT_PER_VERTEX_DATA, 0}
	};

	if(FAILED(D3DX10CreateEffectFromFile(L"effect.fx", 0, 0, 
		"fx_4_0", shaderFlags, 0, device, 0, 0, &Effects::MeshFX, &compilationErrors, 0)))
	{
		if( compilationErrors )
		{
			MessageBoxA(0, (char*)compilationErrors->GetBufferPointer(), 0, 0);
			compilationErrors->Release();
			compilationErrors = 0;
		}
	}

	// Create the input layout
	MeshFX->GetTechniqueByName("Tech")->GetPassByIndex(0)->GetDesc(&PassDesc);
	device->CreateInputLayout(vertexDesc, 4, PassDesc.pIAInputSignature, 
		PassDesc.IAInputSignatureSize, &PosTangentNormalTex);

	//*****************************************//
	
	// 
	// Particle vertex
	//
	D3D10_INPUT_ELEMENT_DESC particleDesc[] =
	{
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,  0, D3D10_INPUT_PER_VERTEX_DATA, 0},
		{"VELOCITY", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D10_INPUT_PER_VERTEX_DATA, 0},
		{"SIZE",     0, DXGI_FORMAT_R32G32_FLOAT,    0, 24, D3D10_INPUT_PER_VERTEX_DATA, 0},
		{"AGE",      0, DXGI_FORMAT_R32_FLOAT,       0, 32, D3D10_INPUT_PER_VERTEX_DATA, 0},
		{"TYPE",     0, DXGI_FORMAT_R32_UINT,        0, 36, D3D10_INPUT_PER_VERTEX_DATA, 0},
	};

	//Fire
	if(FAILED(D3DX10CreateEffectFromFile(L"fire.fx", 0, 0, 
		"fx_4_0", shaderFlags, 0, device, 0, 0, &Effects::FireFX, &compilationErrors, 0)))
	{
		if( compilationErrors )
		{
			MessageBoxA(0, (char*)compilationErrors->GetBufferPointer(), 0, 0);
			compilationErrors->Release();
			compilationErrors = 0;
		}
	}

	FireFX->GetTechniqueByName("StreamOutTech")->GetPassByIndex(0)->GetDesc(&PassDesc);
	device->CreateInputLayout(particleDesc, 5, PassDesc.pIAInputSignature,
		PassDesc.IAInputSignatureSize, &Particle);

	//Rain
	if(FAILED(D3DX10CreateEffectFromFile(L"rain.fx", 0, 0, 
		"fx_4_0", shaderFlags, 0, device, 0, 0, &Effects::RainFX, &compilationErrors, 0)))
	{
		if( compilationErrors )
		{
			MessageBoxA(0, (char*)compilationErrors->GetBufferPointer(), 0, 0);
			compilationErrors->Release();
			compilationErrors = 0;
		}
	}

	RainFX->GetTechniqueByName("StreamOutTech")->GetPassByIndex(0)->GetDesc(&PassDesc);
	device->CreateInputLayout(particleDesc, 5, PassDesc.pIAInputSignature,
		PassDesc.IAInputSignatureSize, &Particle);

	//
	// SkyVertex
	//
	D3D10_INPUT_ELEMENT_DESC skyVertexDesc[] =
	{
		{"POSITION",  0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0,  D3D10_INPUT_PER_VERTEX_DATA, 0}
	};

	if(FAILED(D3DX10CreateEffectFromFile(L"sky.fx", 0, 0, 
		"fx_4_0", shaderFlags, 0, device, 0, 0, &Effects::SkyFX, &compilationErrors, 0)))
	{
		if( compilationErrors )
		{
			MessageBoxA(0, (char*)compilationErrors->GetBufferPointer(), 0, 0);
			compilationErrors->Release();
			compilationErrors = 0;
		}
	}

	SkyFX->GetTechniqueByName("SkyTech")->GetPassByIndex(0)->GetDesc(&PassDesc);
	device->CreateInputLayout(skyVertexDesc, 1, PassDesc.pIAInputSignature,
		PassDesc.IAInputSignatureSize, &Pos);

	//*****************************************//

	//
	// Billboard
	//
	D3D10_INPUT_ELEMENT_DESC billboardVertexDesc[] =
	{
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D10_INPUT_PER_VERTEX_DATA, 0},
		{"SIZE",     0, DXGI_FORMAT_R32G32_FLOAT,    0, 12, D3D10_INPUT_PER_VERTEX_DATA, 0},
	};

	if(FAILED(D3DX10CreateEffectFromFile(L"billboard.fx", 0, 0, 
		"fx_4_0", shaderFlags, 0, device, 0, 0, &Effects::BillboardFX, &compilationErrors, 0)))
	{
		if( compilationErrors )
		{
			MessageBoxA(0, (char*)compilationErrors->GetBufferPointer(), 0, 0);
			compilationErrors->Release();
			compilationErrors = 0;
		}
	}

	BillboardFX->GetTechniqueByName("BillboardTech")->GetPassByIndex(0)->GetDesc(&PassDesc);
	device->CreateInputLayout(billboardVertexDesc, 2, PassDesc.pIAInputSignature,
		PassDesc.IAInputSignatureSize, &PosSize);

	//*****************************************//

	//
	//	AABB
	//
	D3D10_INPUT_ELEMENT_DESC AABBDesc[] =
	{
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0,  D3D10_INPUT_PER_VERTEX_DATA, 0},
		{"COLOR",   0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D10_INPUT_PER_VERTEX_DATA, 0}
	};

	if(FAILED(D3DX10CreateEffectFromFile(L"grid.fx", 0, 0, 
		"fx_4_0", shaderFlags, 0, device, 0, 0, &Effects::AABBFX, &compilationErrors, 0)))
	{
		if( compilationErrors )
		{
			MessageBoxA(0, (char*)compilationErrors->GetBufferPointer(), 0, 0);
			compilationErrors->Release();
			compilationErrors = 0;
		}
	}

	// Create the input layout
	AABBFX->GetTechniqueByName("Render")->GetPassByIndex(0)->GetDesc(&PassDesc);
	device->CreateInputLayout(AABBDesc, 2, PassDesc.pIAInputSignature, 
		PassDesc.IAInputSignatureSize, &PosColor);

	//*****************************************//
}

void Effects::DestroyAll()
{
	PosTangentNormalTex->Release();
	Particle->Release();
	Pos->Release();
	PosTangentNormalTex = 0;
	Particle = 0;
	Pos = 0;
}