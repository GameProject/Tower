#include "PSystem.h"

PSystem::PSystem()
{
	md3dDevice	 = 0;
	mDrawVB		 = 0;
	mStreamOutVB = 0;
	mTexArrayRV  = 0;
	mRandomTexRV = 0;

	mFirstRun = true;
	mGameTime = 0.0f;
	mTimeStep = 0.0f;
	mAge      = 0.0f;
}

PSystem::~PSystem()
{
	mDrawVB->Release();
	mStreamOutVB->Release();
	mDrawVB = 0;
	mStreamOutVB = 0;
}

float PSystem::RandF()
{
	return (float)(rand()) / (float)RAND_MAX;
}

// Returns random float in [a, b).
float PSystem::RandF(float a, float b)
{
	return a + RandF()*(b-a);
}

void PSystem::init(ID3D10Device* device, ID3D10Effect* FX, std::wstring texRV, UINT maxParticles)
{
	md3dDevice = device;
	mMaxParticles = maxParticles;

	D3DX10CreateShaderResourceViewFromFile(md3dDevice, texRV.c_str(), 0, 0, &mTexArrayRV, 0 );

	D3D10_TEXTURE1D_DESC texDesc;
	texDesc.Width = 1024;
	texDesc.MipLevels = 1;
	texDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	texDesc.Usage = D3D10_USAGE_IMMUTABLE;
	texDesc.BindFlags = D3D10_BIND_SHADER_RESOURCE;
	texDesc.CPUAccessFlags = 0;
	texDesc.MiscFlags = 0;
	texDesc.ArraySize = 1;

	D3DXVECTOR4 randomValues[1024];
	for(int i = 0; i < 1024; ++i)
	{
		randomValues[i].x = RandF(-1.0f, 1.0f);
		randomValues[i].y = RandF(-1.0f, 1.0f);
		randomValues[i].z = RandF(-1.0f, 1.0f);
		randomValues[i].w = RandF(-1.0f, 1.0f);
	}

	D3D10_SUBRESOURCE_DATA initData;
	initData.pSysMem = randomValues;
	initData.SysMemPitch = 1024*sizeof(D3DXVECTOR4);
	initData.SysMemSlicePitch = 1024*sizeof(D3DXVECTOR4);

	ID3D10Texture1D* randomTex = 0;
	md3dDevice->CreateTexture1D(&texDesc, &initData, &randomTex);

	D3D10_SHADER_RESOURCE_VIEW_DESC viewDesc;
	viewDesc.Format = texDesc.Format;
	viewDesc.ViewDimension = D3D10_SRV_DIMENSION_TEXTURE1D;
	viewDesc.Texture1D.MipLevels = texDesc.MipLevels;
	viewDesc.Texture1D.MostDetailedMip = 0;

	md3dDevice->CreateShaderResourceView(randomTex, &viewDesc, &mRandomTexRV);

	mStreamOutTech  = FX->GetTechniqueByName("StreamOutTech");	
	mDrawTech       = FX->GetTechniqueByName("DrawTech");	
	mfxViewProjVar  = FX->GetVariableByName("gViewProj")->AsMatrix();
	mfxGameTimeVar  = FX->GetVariableByName("gGameTime")->AsScalar();
	mfxTimeStepVar  = FX->GetVariableByName("gTimeStep")->AsScalar();
	mfxEyePosVar    = FX->GetVariableByName("gEyePosW")->AsVector();
	mfxEmitPosVar   = FX->GetVariableByName("gEmitPosW")->AsVector();
	mfxEmitDirVar   = FX->GetVariableByName("gEmitDirW")->AsVector();
	mfxTexArrayVar  = FX->GetVariableByName("gTexArray")->AsShaderResource();
	mfxRandomTexVar = FX->GetVariableByName("gRandomTex")->AsShaderResource();

	buildVB();
}

void PSystem::buildVB()
{
	ParticleVertex p;
	p.age  = 0.0f;
	p.type = 0;

	D3D10_BUFFER_DESC vbd;
	vbd.Usage = D3D10_USAGE_DEFAULT;
	vbd.ByteWidth = sizeof(ParticleVertex) * mMaxParticles;
	vbd.BindFlags = D3D10_BIND_VERTEX_BUFFER | D3D10_BIND_STREAM_OUTPUT;
	vbd.CPUAccessFlags = 0;
	vbd.MiscFlags = 0;
	md3dDevice->CreateBuffer(&vbd, 0, &mDrawVB);
	md3dDevice->CreateBuffer(&vbd, 0, &mStreamOutVB);
}

void PSystem::setEyePos(const D3DXVECTOR3& eyePosW)
{
	mEyePosW = D3DXVECTOR4(eyePosW.x, eyePosW.y, eyePosW.z, 1.0f);
}

void PSystem::setEmitPos(const D3DXVECTOR3& emitPosW)
{
	mEmitPosW = D3DXVECTOR4(emitPosW.x, emitPosW.y, emitPosW.z, 1.0f);
}

void PSystem::setEmitDir(const D3DXVECTOR3& emitDirW)
{
	mEmitDirW = D3DXVECTOR4(emitDirW.x, emitDirW.y, emitDirW.z, 0.0f);
}

void PSystem::reset()
{
	mFirstRun = true;
	mAge      = 0.0f;
}

void PSystem::update(float dt, float gameTime)
{
	mGameTime = gameTime;
	mTimeStep = dt;

	mAge += dt;
}

float PSystem::getAge()const
{
	return mAge;
}

void PSystem::draw(D3DXMATRIX v, D3DXMATRIX p)
{
	D3DXMATRIX V = v;
	D3DXMATRIX P = p;

	mfxViewProjVar->SetMatrix((float*)&(V*P));
	mfxGameTimeVar->SetFloat(mGameTime);
	mfxTimeStepVar->SetFloat(mTimeStep);
	mfxEyePosVar->SetFloatVector((float*)&mEyePosW);
	mfxEmitPosVar->SetFloatVector((float*)&mEmitPosW);
	mfxEmitDirVar->SetFloatVector((float*)&mEmitDirW);
	mfxTexArrayVar->SetResource(mTexArrayRV);
	mfxRandomTexVar->SetResource(mRandomTexRV);

	md3dDevice->IASetInputLayout(Effects::Particle);
	md3dDevice->IASetPrimitiveTopology(D3D10_PRIMITIVE_TOPOLOGY_POINTLIST);

	UINT stride = sizeof(ParticleVertex);
	UINT offset = 0;

	md3dDevice->IASetVertexBuffers(0, 1, &mDrawVB, &stride, &offset);
	md3dDevice->SOSetTargets(1, &mStreamOutVB, &offset);

	D3D10_TECHNIQUE_DESC techDesc;
	mStreamOutTech->GetDesc( &techDesc );
	for(UINT p = 0; p < techDesc.Passes; ++p)
	{
		mStreamOutTech->GetPassByIndex( p )->Apply(0);

		if( mFirstRun )
		{
			md3dDevice->Draw(1, 0);
			mFirstRun = false;
		}
		else
		{
			md3dDevice->DrawAuto();
		}
	}
	ID3D10Buffer* bufferArray[1] = {0};
	md3dDevice->SOSetTargets(1, bufferArray, &offset);

	std::swap(mDrawVB, mStreamOutVB);
	md3dDevice->IASetVertexBuffers(0, 1, &mDrawVB, &stride, &offset);

	mDrawTech->GetDesc( &techDesc );
	for(UINT p = 0; p < techDesc.Passes; ++p)
	{
		mDrawTech->GetPassByIndex( p )->Apply(0);
		md3dDevice->DrawAuto();
	}
}