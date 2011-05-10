struct Light
{
	float3 pos;
	float3 dir;
	float4 ambient;
	float4 diffuse;
	float4 spec;
	float3 att;
	float  spotPower;
	float  range;
};

cbuffer cbPerFrame
{
	float3 gEyePos;
	Light gLight;
};

cbuffer cbPerObject
{
	matrix gWVP;
	matrix gWorld;
	matrix gTexMtx;
	int gType;
	int gLightType;
};

cbuffer cbFixed
{
	// For this demo, we hardcode the fog values.  However, in a real
	// application, the program may want to change the fog settings
	// at runtime; for example, to fade the fog in and out based on 
	// the time of day or the location of the game player.
 	
 	float  gFogRange = 80.0f; //50.0f if sky = gFogColor
	float  gFogStart = 5.0f;
	float3 gFogColor = {0.7f, 0.7f, 0.7f};
};

Texture2D gDiffuseMap;
Texture2D gSpecMap;
Texture2D gNormalMap;

Texture2D gBlendMap;
Texture2D gBlend1Map;
Texture2D gBlend2Map;
Texture2D gBlend3Map;

SamplerState textureSampler
{
	Filter = ANISOTROPIC;
	AddressU = Wrap;
	AddressV = Wrap;
};

struct VS_IN
{
	float4 pos		: POSITION;
	float3 normal	: NORMAL;
	float2 texC     : TEXCOORD;
	float3 tangent  : TANGENT;
};

struct VS_OUT
{
	float4 posH		: SV_POSITION;
	float4 posW		: POSITION;
	float3 tangent  : TANGENT;
	float3 normal	: NORMAL;
	float2 texC     : TEXCOORD0;
	float2 texC2    : TEXCOORD1;
	float  fogLerp  : FOG;
};

struct GS_OUT
{
	float4 posH		: SV_POSITION;
	float4 posW		: POSITION;
	float3 tangent  : TANGENT;
	float3 normal	: NORMAL;
	float2 texC		: TEXCOORD0;
	float2 texC2	: TEXCOORD1;
	float  fogLerp	: FOG;
};

DepthStencilState EnableDepth
{
    DepthEnable = TRUE;
};
 
VS_OUT VS(VS_IN vIn)
{
	VS_OUT vOut;
	
	vOut.posW = mul(vIn.pos, gWorld);
	vOut.posH = vIn.pos;
	vOut.tangent = vIn.tangent;
	vOut.normal = vIn.normal;
	vOut.texC = mul(vIn.texC, gTexMtx);
	vOut.texC2 = vIn.texC;
	
	float d = distance(vOut.posW, gEyePos);
	vOut.fogLerp = ( (d - gFogStart) / gFogRange ); 
	
	return vOut;
}

[maxvertexcount(6)]
void GS(triangle VS_OUT gIn[3], inout TriangleStream<GS_OUT> triStream)
{
	GS_OUT gOut;
				
	float3 e1 = gIn[1].posW - gIn[0].posW;
	float3 e2 = gIn[2].posW - gIn[0].posW;
	float4 normal = float4(normalize( cross(e1, e2) ), 0.0f);
	gOut.normal = mul(normal, gWorld);
	
	for (int i=0; i<3; ++i)
	{
		gOut.texC2 = gIn[i].texC2;
		gOut.texC = gIn[i].texC;
		gOut.posH = mul(gIn[i].posH, gWVP);
		gOut.posW = gIn[i].posW;
		gOut.tangent = gIn[i].tangent;
		gOut.fogLerp = gIn[i].fogLerp;
		triStream.Append(gOut);
	}
}

float4 PS(GS_OUT pIn) : SV_Target
{
	float4 diffuse = gDiffuseMap.Sample( textureSampler, pIn.texC );
//	if (length(pIn.posW - gEyePos) > gFogRange + gFogStart)
//		clip(-diffuse);
	float4 spec	   = gSpecMap.Sample( textureSampler, pIn.texC );
	float3 normalT = gNormalMap.Sample( textureSampler, pIn.texC );
	
	//--Terrain---//
	if (gType == 0)
	{
		diffuse		= gDiffuseMap.Sample( textureSampler, pIn.texC*32 );
		float4 c1	= gBlend1Map.Sample( textureSampler, pIn.texC*32 );
		float4 c2	= gBlend2Map.Sample( textureSampler, pIn.texC*32 );
		float4 c3	= gBlend3Map.Sample( textureSampler, pIn.texC*32 );
		
		float4 t	= gBlendMap.Sample( textureSampler, pIn.texC2);
		
		diffuse = lerp(diffuse, c1, t.r*1.4f);
		diffuse = lerp(diffuse, c2, t.g*1.4f);
		diffuse = lerp(diffuse, c3, t.b*1.4f);
	}
	
	pIn.tangent = pIn.tangent - dot(pIn.tangent, pIn.normal)*pIn.normal;

	//--BumpedNormal----//
	normalT = 2.0f*normalT - 1.0f;
	
	float3 B = cross(pIn.normal, pIn.tangent);
	float3x3 TBN = float3x3(pIn.tangent, B, pIn.normal);

	float3 bumpedNormalW = normalize(mul(normalT, TBN));
	//---------------//
		
	float3 lightVec;
	float d;
	
	if (gLightType == 1)
		lightVec = -gLight.dir;
	else
	{
		lightVec = gLight.pos - pIn.posW;
		d = length(lightVec);
 		if (d > gLight.range )
 			return float4(0.0f, 0.0f, 0.0f, diffuse.a);
		lightVec /= d;
	}

	//--Diffuse----//
	float diffuseFactor;
	if (gType != 4)
		diffuseFactor = dot(lightVec, bumpedNormalW);
	else
		diffuseFactor = dot(lightVec, pIn.normal);
	diffuseFactor = saturate(diffuseFactor) * gLight.diffuse;
	//---------//
	
	//--Ambient---//
	float3 litColor = diffuse * (diffuseFactor + gLight.ambient);

	//--Specular---//
	if (diffuseFactor > 0.0f && gType != 0 && gType != 4)
	{
		spec.a *= 256.0f;
		float specPower = max(spec.a, 1.0f);
		float3 reflectVec = reflect(-lightVec, bumpedNormalW);
		float3 toEye = normalize(gEyePos - pIn.posW);
		float specFactor = pow(max(dot(reflectVec, toEye), 0.0f), specPower);
		
		litColor += specFactor * spec * gLight.spec;
	}
	//----------//
		
	//--DirectionalLight--//
	if (gLightType == 1)
		return float4(litColor, diffuse.a);
	else
	{
		//--PointLight--//
		litColor /= dot(gLight.att, float3(1.0f, d, d*d));
		
		//--SpotLight--//
		if (gLightType == 3)
			litColor *= pow(max(dot(-lightVec, gLight.dir), 0.0f), gLight.spotPower);
		float3 foggedColor = lerp(litColor, gFogColor, pIn.fogLerp);
		return float4(foggedColor, diffuse.a);
	}
}

RasterizerState noCull
{
	FillMode = Solid;//WireFrame;//
	CullMode = None;//Back;//Front;//
};

technique10 Tech
{
    pass P0
    {
		SetRasterizerState(noCull);
		
        SetVertexShader( CompileShader( vs_4_0, VS() ) );
        SetGeometryShader( CompileShader( gs_4_0, GS() ) );
        SetPixelShader( CompileShader( ps_4_0, PS() ) );
        
        SetDepthStencilState( EnableDepth, 0 );
    }
}