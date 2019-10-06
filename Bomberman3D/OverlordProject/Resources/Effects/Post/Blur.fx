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
	// Step 1: find the dimensions of the texture (the texture has a method for that)
    uint width;
    uint height;
    gTexture.GetDimensions(width, height);
	// Step 2: calculate dx and dy (UV space for 1 pixel)	
    float dx = 1.0f /  width;
    float dy = 1.0f / height;

    //70
	// Step 3: Create a double for loop (5 iterations each)
	//		   Inside the loop, calculate the offset in each direction. Make sure not to take every pixel but move by 2 pixels each time
	//			Do a texture lookup using your previously calculated uv coordinates + the offset, and add to the final color
    float passes;
    float3 finalColor;
    for (int i = 0; i < 5; ++i)
    {
        for (int j = 0; j < 5; ++j)
        {
            float2 offset = float2(input.TexCoord.x - (2 * dx) + (j * dx), input.TexCoord.y + (2 * dy) - (i * dy));
            if (offset.x >= 0 && offset.y >= 0) //80
            {
                ++passes;
                finalColor += gTexture.Sample(samPoint, offset).rgb;
            }
        }
    }
	// Step 4: Divide the final color by the number of passes (in this case 5*5)	
    finalColor.r /= passes;
    finalColor.g /= passes;
    finalColor.b /= passes;
	// Step 5: return the final color

    return float4(finalColor, 1.0f);
}


//TECHNIQUE
//---------
technique11 Blur
{
    pass P0
    {
		// Set states...
        SetVertexShader( CompileShader( vs_4_0, VS() ) );
        SetGeometryShader( NULL );
        SetPixelShader( CompileShader( ps_4_0, PS() ) );
    }
}