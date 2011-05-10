cbuffer cbPerFrame
{
	float3 gEyePosW;
	float4x4 gViewProj; 
};

float2 gQuadTexC[4] = 
{
	float2(0.0f, 1.0f),
	float2(1.0f, 1.0f),
	float2(0.0f, 0.0f),
	float2(1.0f, 0.0f)
};

Texture2DArray gDiffuseMapArray;
 
SamplerState textureSampler
{
	Filter = ANISOTROPIC;
	AddressU = Wrap;
	AddressV = Wrap;
};

struct VS_IN
{
	float3 centerW : POSITION;
	float2 sizeW   : SIZE;
};

struct VS_OUT
{
	float3 centerW : POSITION;
	float2 sizeW   : SIZE;
};

struct GS_OUT
{
	float4 posH    : SV_POSITION;
    float3 posW    : POSITION;
    float3 normalW : NORMAL;
    float2 texC    : TEXCOORD;
    uint primID    : SV_PrimitiveID;
};
 
VS_OUT VS(VS_IN vIn)
{
	VS_OUT vOut;
	
	vOut.centerW  = vIn.centerW;
	vOut.sizeW    = vIn.sizeW;

	return vOut;
}

[maxvertexcount(4)]
void GS(point VS_OUT gIn[1], inout TriangleStream<GS_OUT> triStream)
{
	float halfWidth  = 0.5f*gIn[0].sizeW.x;
	float halfHeight = 0.5f*gIn[0].sizeW.y;
	
	float4 v[4];
	v[0] = float4(-halfWidth, -halfHeight, 0.0f, 1.0f);
	v[1] = float4(halfWidth, -halfHeight, 0.0f, 1.0f);
	v[2] = float4(-halfWidth, halfHeight, 0.0f, 1.0f);
	v[3] = float4(halfWidth, halfHeight, 0.0f, 1.0f);
	
	float3 look  = normalize(gEyePosW.xyz - gIn[0].centerW);
	float3 up    = float3(0,1,0);
	float3 right = normalize(cross(up, look));
	
	
	matrix W;
	W[0] = float4(right,          0.0f);
	W[1] = float4(up,             0.0f);
	W[2] = float4(look,           0.0f);
	W[3] = float4(gIn[0].centerW, 1.0f);

	matrix WVP = mul(W, gViewProj);
	
	GS_OUT gOut;
	for(int i = 0; i < 4; ++i)
	{
		gOut.posH     = mul(v[i], WVP);
		gOut.posW     = mul(v[i], W);
		gOut.normalW  = look;
		gOut.texC     = gQuadTexC[i];
		gOut.primID   = i;
		triStream.Append(gOut);
	}
}

float4 PS(GS_OUT pIn) : SV_Target
{
	float3 uvw = float3(pIn.texC, pIn.primID);
	float4 diffuse = gDiffuseMapArray.Sample( textureSampler, uvw );
	clip(diffuse.a - 0.25f);
    
    return diffuse;
}
 
technique10 BillboardTech
{
    pass P0
    {
        SetVertexShader( CompileShader(   vs_4_0, VS() ) );
        SetGeometryShader( CompileShader( gs_4_0, GS() ) );
        SetPixelShader( CompileShader(    ps_4_0, PS() ) );
    }
}
