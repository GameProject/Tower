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
	float3 gAccelW = {0.0f, 5.0f, 0.0f};
};

float2 gQuadTexC[4] = 
{
	float2(0.0f, 1.0f),
	float2(1.0f, 1.0f),
	float2(0.0f, 0.0f),
	float2(1.0f, 0.0f)
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

BlendState Blending
{
    BlendEnable[0] = TRUE;
    SrcBlend = SRC_ALPHA;
    DestBlend = ONE;
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

[maxvertexcount(2)]
void StreamOutGS(point VS_IN gIn[1], inout PointStream<VS_IN> ptStream)
{	
	gIn[0].age += gTimeStep;
	
	if( gIn[0].type == 0 )
	{	
		// time to emit a new particle?
		if( gIn[0].age > 0.005f )
		{
			float3 vRandom = normalize(gRandomTex.SampleLevel(textureSampler, gGameTime, 0));
			vRandom.x *= 0.5f;
			vRandom.z *= 0.5f;
			   
			VS_IN p;
			p.initialPosW = gEmitPosW;
			p.initialVelW = 3.0f*vRandom;
			p.sizeW       = float2(4.0f, 4.0f);
			p.age         = 0.0f;
			p.type        = 1;
			ptStream.Append(p);
			
			// reset the time to emit
			gIn[0].age = 0.0f;
		}
		
		// always keep emitters
		ptStream.Append(gIn[0]);
	}
	else
	{
		// Specify conditions to keep particle.
		//this may vary from system to system.
		if( gIn[0].age <= 1.0f )
			ptStream.Append(gIn[0]);
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
	float2 sizeW : SIZE;
	float4 color : COLOR;
	uint   type  : TYPE;
};

VS_OUT DrawVS(VS_IN vIn)
{
	VS_OUT vOut;
	
	// constant acceleration equation
	vOut.posW = 0.5f*vIn.age*vIn.age*gAccelW + vIn.age*vIn.initialVelW + vIn.initialPosW;
	
	// fade color with time
	float opacity = 1.0f - smoothstep(0.0f, 1.0f, vIn.age);
	if (opacity < 0.98f)
		vOut.color = float4(1.0f, 1.0f, 1.0f, opacity);
	else
		vOut.color = float4(0.1f, 0.8f, 1.0f, opacity);
	
	vOut.sizeW.x = vIn.sizeW.x-vOut.posW.y;
	vOut.sizeW.y = vIn.sizeW.y;
	vOut.type  = vIn.type;
	
	return vOut;
}

struct GS_OUT
{
	float4 posH  : SV_Position;
	float4 color : COLOR;
	float2 texC  : TEXCOORD;
};

[maxvertexcount(4)]
void DrawGS(point VS_OUT gIn[1], inout TriangleStream<GS_OUT> triStream)
{	
	// do not draw emitter particles.
	if( gIn[0].type != 0 )
	{		
		float halfWidth  = 0.5f*gIn[0].sizeW.x;
		float halfHeight = 0.5f*gIn[0].sizeW.y;
	
		float4 v[4];
		v[0] = float4(-halfWidth, -halfHeight, 0.0f, 1.0f);
		v[1] = float4(halfWidth, -halfHeight, 0.0f, 1.0f);
		v[2] = float4(-halfWidth, halfHeight, 0.0f, 1.0f);
		v[3] = float4(halfWidth, halfHeight, 0.0f, 1.0f);
		
		float3 look  = normalize(gEyePosW.xyz - gIn[0].posW);
		float3 right = normalize(cross(float3(0,1,0), look));
		float3 up    = cross(look, right);
		
		float4x4 W;
		W[0] = float4(right,       0.0f);
		W[1] = float4(up,          0.0f);
		W[2] = float4(look,        0.0f);
		W[3] = float4(gIn[0].posW, 1.0f);

		float4x4 WVP = mul(W, gViewProj);
		
		GS_OUT gOut;
		for(int i = 0; i < 4; ++i)
		{
			gOut.posH  = mul(v[i], WVP);
			gOut.texC  = gQuadTexC[i];
			gOut.color = gIn[0].color;
			triStream.Append(gOut);
		}	
	}
}

float4 DrawPS(GS_OUT pIn) : SV_TARGET
{
	return gTexArray.Sample(textureSampler, float3(pIn.texC, 0))*pIn.color;
}

technique10 DrawTech
{
    pass P0
    {
        SetVertexShader(   CompileShader( vs_4_0, DrawVS() ) );
        SetGeometryShader( CompileShader( gs_4_0, DrawGS() ) );
        SetPixelShader(    CompileShader( ps_4_0, DrawPS() ) );
        
        SetBlendState(Blending, float4(0.0f, 0.0f, 0.0f, 0.0f), 0xffffffff);
        SetDepthStencilState( NoDepthWrites, 0 );
    }
}