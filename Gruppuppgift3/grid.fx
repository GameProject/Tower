cbuffer cbPerObject
{
	matrix World;
	matrix WVP;
}

struct PS_INPUT
{
	float4 Pos : SV_POSITION;
    float4 Color : COLOR0;
};

PS_INPUT VS( float4 Pos : POSITION, float4 Color : COLOR )
{
	PS_INPUT psInput;

    psInput.Pos = mul( Pos, WVP );
	psInput.Color = Color;
	
    return psInput;  
}

float4 PS( PS_INPUT psInput ) : SV_Target
{
    return psInput.Color; 
}

DepthStencilState EnableDepth
{
    DepthEnable = TRUE;
};

technique10 Render
{
    pass P0
    {
        SetVertexShader( CompileShader( vs_4_0, VS() ) );
        SetGeometryShader( NULL );
        SetPixelShader( CompileShader( ps_4_0, PS() ) );
        
        SetDepthStencilState( EnableDepth, 0 );
    }
}

