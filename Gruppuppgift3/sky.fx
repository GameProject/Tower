cbuffer cbPerObject
{
	matrix gWVP;
	int gLightType;
};

TextureCube gCubeMap;

struct Light
{
	float4 ambient;
	float4 diffuse;
};

SamplerState gTriLinearSam
{
	Filter = ANISOTROPIC;
	AddressU = Wrap;
	AddressV = Wrap;
};

struct VS_IN
{
	float3 pos : POSITION;
};

struct VS_OUT
{
	float4 posH : SV_POSITION;
    float3 texC : TEXCOORD;
};
 
VS_OUT VS(VS_IN vIn)
{
	VS_OUT vOut;
	
	vOut.posH = mul(float4(vIn.pos, 0.0f), gWVP).xyww;
	vOut.texC = vIn.pos;
	
	return vOut;
}

float4 PS(VS_OUT pIn) : SV_Target
{
	float4 diffuse = gCubeMap.Sample(gTriLinearSam, pIn.texC);
	
	Light gLight;
	if (gLightType == 1)
	{
		gLight.ambient = float4(0.6f, 0.6f, 0.6f, 1.0f);
		gLight.diffuse = float4(0.6f, 0.6f, 0.6f, 1.0f);
	}
	else
	{
		gLight.ambient = float4(0.4f, 0.3f, 0.3f, 1.0f);
		gLight.diffuse = float4(0.4f, 0.4f, 0.4f, 1.0f);
	}
	float3 litColor = diffuse * (gLight.diffuse + gLight.ambient);	

	return float4(litColor, diffuse.a);
}

DepthStencilState LessEqualDSS
{
    DepthFunc = LESS_EQUAL;
};

technique10 SkyTech
{
    pass P0
    {
        SetVertexShader( CompileShader( vs_4_0, VS() ) );
        SetGeometryShader( NULL );
        SetPixelShader( CompileShader( ps_4_0, PS() ) );

        SetDepthStencilState(LessEqualDSS, 0);
    }
}
