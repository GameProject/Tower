cbuffer cbPerFrame
{
	float4 gEyePosW;
	float4 gEmitPosW;
	float4 gEmitDirW;
	float gGameTime;
	float gTimeStep;
};

cbuffer cbPerObject
{
	matrix gViewProj; 
	float3 gAccelW = {-1.0f, -30.0f, 1.0f};
};

Texture2DArray gTexArray;
Texture1D gRandomTex;
 
SamplerState textureSampler
{
	Filter = ANISOTROPIC;
	AddressU = WRAP;
	AddressV = WRAP;
};

DepthStencilState NoDepthWrites
{
    DepthEnable = TRUE;
    DepthWriteMask = ZERO;
};

struct VS_IN
{
	float3 initialPosW : POSITION;
	float3 initialVelW : VELOCITY;
	float2 sizeW       : SIZE;
	float age          : AGE;
	uint type          : TYPE;
};
  
VS_IN StreamOutVS(VS_IN vIn)
{
	return vIn;
}

[maxvertexcount(6)]
void StreamOutGS(point VS_IN gIn[1], inout PointStream<VS_IN> ptStream)
{	
	gIn[0].age += gTimeStep;
	
	if( gIn[0].type == 0 )
	{	
		// time to emit a new particle?
		if( gIn[0].age > 0.001f )
		{
			for(int i = 0; i < 5; ++i)
			{
				// Spread rain drops out above the camera.
				float u = (gGameTime + ((float)i/5.0f));
				float3 vRandom = 35.0f*gRandomTex.SampleLevel(textureSampler, u, 0);
				vRandom.y = 20.0f;
			
				VS_IN p;
				p.initialPosW = gEmitPosW + vRandom;
				p.initialVelW = float3(0.0f, 0.0f, 0.0f);
				p.sizeW       = float2(0.8f, 0.5f);
				p.age         = 0.0f;
				p.type        = 1;
				ptStream.Append(p);
			}
			
			// reset the time to emit
			gIn[0].age = 0.0f;
		}
		ptStream.Append(gIn[0]);
	}
	else
	{
		// Specify conditions to keep particle; this may vary from system to system.
		if( gIn[0].age <= 3.0f )
		{
			if (gIn[0].sizeW.y < 1.0f)
				gIn[0].sizeW.y += gIn[0].age;
			ptStream.Append(gIn[0]);
		}
	}		
}

GeometryShader gsStreamOut = ConstructGSWithSO( 
	CompileShader( gs_4_0, StreamOutGS() ), 
	"POSITION.xyz; VELOCITY.xyz; SIZE.xy; AGE.x; TYPE.x" );
	
technique10 StreamOutTech
{
    pass P0
    {
        SetVertexShader( CompileShader( vs_4_0, StreamOutVS() ) );
        SetGeometryShader( gsStreamOut );
        SetPixelShader(NULL);
    }
}

struct VS_OUT
{
	float3 posW  : POSITION;
	uint   type  : TYPE;
};

VS_OUT DrawVS(VS_IN vIn)
{
	VS_OUT vOut;
	
	vOut.posW = 0.5f*vIn.age*vIn.age*gAccelW + vIn.age*vIn.initialVelW + vIn.initialPosW;
	vOut.type  = vIn.type;
	
	return vOut;
}

struct GS_OUT
{
	float4 posH  : SV_Position;
	float2 texC  : TEXCOORD;
};

[maxvertexcount(2)]
void DrawGS(point VS_OUT gIn[1], 
            inout LineStream<GS_OUT> lineStream)
{	
	// do not draw emitter particles.
	if( gIn[0].type != 0 )
	{
		// Slant line in acceleration direction.
		float3 p0 = gIn[0].posW;
		float3 p1 = gIn[0].posW + 0.07f*gAccelW;
		
		GS_OUT v0;
		v0.posH = mul(float4(p0, 1.0f), gViewProj);
		v0.texC = float2(0.0f, 0.0f);
		lineStream.Append(v0);
		
		GS_OUT v1;
		v1.posH = mul(float4(p1, 1.0f), gViewProj);
		v1.texC = float2(1.0f, 1.0f);
		lineStream.Append(v1);
	}
}

float4 DrawPS(GS_OUT pIn) : SV_TARGET
{
	return gTexArray.Sample(textureSampler, float3(pIn.texC, 0));
}

technique10 DrawTech
{
    pass P0
    {
        SetVertexShader(   CompileShader( vs_4_0, DrawVS() ) );
        SetGeometryShader( CompileShader( gs_4_0, DrawGS() ) );
        SetPixelShader(    CompileShader( ps_4_0, DrawPS() ) );
        
        SetDepthStencilState( NoDepthWrites, 0 );
    }
}