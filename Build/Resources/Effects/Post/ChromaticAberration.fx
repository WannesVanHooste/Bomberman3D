//=============================================================================
//// Shader uses position and texture
//=============================================================================
SamplerState samPoint
{
    Filter = MIN_MAG_MIP_POINT;
    AddressU = Mirror;
    AddressV = Mirror;
};
//10
Texture2D gTexture;

/// Create Depth Stencil State (ENABLE DEPTH WRITING)
DepthStencilState EnableDepthWriting
{
    DepthEnable = TRUE;
    DepthWriteMask = ALL;
};

//20
/// Create Rasterizer State (Backface culling) 
RasterizerState BackCulling
{
    CullMode = BACK;
};


//IN/OUT STRUCTS
//--------------
struct VS_INPUT    //30
{
    float3 Position : POSITION;
	float2 TexCoord : TEXCOORD0;

};

struct PS_INPUT
{
    float4 Position : SV_POSITION;
	float2 TexCoord : TEXCOORD1;  //40
};


//VERTEX SHADER
//-------------
PS_INPUT VS(VS_INPUT input)
{
	PS_INPUT output = (PS_INPUT)0;
	// Set the Position
    output.Position = float4(input.Position, 1); //50
	// Set the TexCoord
    output.TexCoord = input.TexCoord;
	
	return output;
}


//PIXEL SHADER
//------------
float4 PS(PS_INPUT input): SV_Target //60
{
    float3 finalColor;
    uint width;
    uint height;
    gTexture.GetDimensions(width, height);

    //space for 1 pixel
    float dx = 1.0f /  width;
    float dy = 1.0f / height;

    //color offsets
    float2 offsetR = float2(input.TexCoord.x + (10 * dx), input.TexCoord.y + (10 * dy));
    float2 offsetG = float2(input.TexCoord.x + (2 * dx), input.TexCoord.y + (2 * dy));
    float2 offsetB = float2(input.TexCoord.x - (10 * dx), input.TexCoord.y - (10 * dy));
	//RED CHANNEL
    if(offsetR.x >= 0 && offsetR.y >= 0 && offsetR.x <= width && offsetR.y <= height)
    {
        finalColor.r = gTexture.Sample(samPoint, offsetR).r;
    }
    else
    {
        finalColor.r = gTexture.Sample(samPoint, input.TexCoord).r;
    }
    //GREEN CHANNEL
    if (offsetG.x >= 0 && offsetG.y >= 0 && offsetG.x <= width && offsetG.y <= height)
    {
        finalColor.g = gTexture.Sample(samPoint, offsetG).g;
    }
    else
    {
        finalColor.g = gTexture.Sample(samPoint, input.TexCoord).g;
    }
    //BLUE CHANNEL
    if (offsetB.x >= 0 && offsetB.y >= 0 && offsetB.x <= width && offsetB.y <= height)
    {
        finalColor.b = gTexture.Sample(samPoint, offsetB).b;
    }
    else
    {
        finalColor.b = gTexture.Sample(samPoint, input.TexCoord).b;
    }
	// Step 5: return the final color

    return float4(finalColor, 1.0f);
}


//TECHNIQUE
//---------
technique11 ChromaticAberration
{
    pass P0
    {
		// Set states...
        SetVertexShader( CompileShader( vs_4_0, VS() ) );
        SetGeometryShader( NULL );
        SetPixelShader( CompileShader( ps_4_0, PS() ) );
    }
}