/*
******************
* DAE Ubershader *
******************

**This Shader Contains:

- Diffuse (Texture & Color)
	- Regular Diffuse
- Specular
	- Specular Level (Texture & Value)
	- Shininess (Value)
	- Models
		- Blinn
		- Phong
- Ambient (Color)
- EnvironmentMapping (CubeMap)
	- Reflection + Fresnel Falloff
	- Refraction
- Normal (Texture)
- Opacity (Texture & Value)

-Techniques
	- WithAlphaBlending
	- WithoutAlphaBlending
*/

//GLOBAL MATRICES
//***************
// The World View Projection Matrix
float4x4 gMatrixWVP : WORLDVIEWPROJECTION;
// The ViewInverse Matrix - the third row contains the camera position!
float4x4 gMatrixViewInverse : VIEWINVERSE;
// The World Matrix
float4x4 gMatrixWorld : WORLD;

//STATES
//******
RasterizerState gRS_FrontCulling 
{ 
	CullMode = NONE; 
};

BlendState gBS_EnableBlending 
{     
	BlendEnable[0] = TRUE;
	SrcBlend = SRC_ALPHA;
    DestBlend = INV_SRC_ALPHA;
};

//SAMPLER STATES
//**************
SamplerState gTextureSampler
{
	Filter = MIN_MAG_MIP_LINEAR;
 	AddressU = WRAP;
	AddressV = WRAP;
	AddressW = WRAP;
};

//LIGHT
//*****
float3 gLightDirection : DIRECTION;

//DIFFUSE
//*******
bool gUseTextureDiffuse;

float4 gColorDiffuse;

Texture2D gTextureDiffuse;

//SPECULAR
//********
float4 gColorSpecular;

Texture2D gTextureSpecularIntensity;

bool gUseTextureSpecularIntensity;

int gShininess;

//AMBIENT
//*******
float4 gColorAmbient;

float gAmbientIntensity;

//NORMAL MAPPING
//**************
bool gFlipGreenChannel;

bool gUseTextureNormal;

Texture2D gTextureNormal;

//ENVIRONMENT MAPPING
//*******************
TextureCube gCubeEnvironment;

bool gUseTextureEnvironment;

float gReflectionStrength;

float gRefractionStrength;

float gRefractionIndex;

//FRESNEL FALLOFF
//***************
bool gUseFresnelFalloff;


float4 gColorFresnel;

float gFresnelPower;

float gFresnelMultiplier;

float gFresnelHardness;

//OPACITY
//***************
float gOpacityIntensity = 1.0f;

bool gUseTextureOpacity = false;

Texture2D gTextureOpacity;


//SPECULAR MODELS
//***************
bool gUseSpecularBlinn = false;

bool gUseSpecularPhong = false;

//VS IN & OUT
//***********
struct VS_Input
{
	float3 Position: POSITION;
	float3 Normal: NORMAL;
	float3 Tangent: TANGENT;
	float2 TexCoord: TEXCOORD0;
};

struct VS_Output
{
	float4 Position: SV_POSITION;
	float4 WorldPosition: COLOR0;
	float3 Normal: NORMAL;
	float3 Tangent: TANGENT;
	float2 TexCoord: TEXCOORD0;
};

//DIFFUSE FUNCTION
float3 CalculateDiffuse(float3 normal, float2 texCoord)
{
	float3 diffuseColor = gColorDiffuse;

	//Diffuse Logic
	//...
	
	if(gUseTextureDiffuse)
	{
	diffuseColor = gTextureDiffuse.Sample(gTextureSampler, texCoord);
	float diffuseIntensity = dot(normal, -gLightDirection);
	diffuseIntensity = saturate(diffuseIntensity);
    diffuseColor *= diffuseIntensity;
	}
	else{
		diffuseColor = gColorDiffuse;
	}
	
	return diffuseColor;
}

//SPECULAR FUNCTION (BLINN)
float3 CalculateSpecularBlinn(float3 viewDirection, float3 normal, float2 texCoord)
{
	float3 specularColor = float3(0.0f, 0.0f, 0.0f);
	
	//Specular Blinn Logic
	//...
	
	//Calculate halfvector
    float3 halfVector = -normalize(viewDirection + gLightDirection);
    //Calculate Blinn SpecularStrength
    float specularStrength = dot(halfVector, normal);
	
    specularStrength = saturate(specularStrength);

    specularStrength = pow(specularStrength, gShininess);
    //Multiply SpecularStrength with SpecularColor
	if(gUseTextureSpecularIntensity)
	{
		specularStrength *= gTextureSpecularIntensity.Sample(gTextureSampler, texCoord);
	}
	
    specularColor = gColorSpecular * specularStrength;
	
	return specularColor;
}

//SPECULAR FUNCTION (PHONG)
float3 CalculateSpecularPhong(float3 viewDirection, float3 normal, float2 texCoord)
{
	float3 specularColor = float3(0.0f, 0.0f, 0.0f);
	
	//Specular Specular Logic
	//...
		
		float3 reflectedLightDirection = reflect(gLightDirection, normal);
	float specularStrength = dot(-viewDirection, reflectedLightDirection); 
    specularStrength = saturate(specularStrength); 
    specularStrength = pow(specularStrength, gShininess); 
	
	if(gUseTextureSpecularIntensity)
	{
		specularStrength *= gTextureSpecularIntensity.Sample(gTextureSampler, texCoord);
	}
	
	
    specularColor = (gColorSpecular * specularStrength);
	
	return specularColor;
}

//SPECULAR FUNCTION (SHARED) - finished
float3 CalculateSpecular(float3 viewDirection, float3 normal, float2 texCoord)
{
	float3 specularColor = float3(0,0,0);
	
	if(gUseSpecularBlinn)
		specularColor += CalculateSpecularBlinn(viewDirection, normal, texCoord);
		
	if(gUseSpecularPhong)
		specularColor += CalculateSpecularPhong(viewDirection, normal, texCoord);
				
	return specularColor;
}

//NORMAL MAPPING FUNCTION
float3 CalculateNormal(float3 tangent, float3 normal, float2 texCoord)
{
	float3 newNormal = normal;
	
	//Normalmapping Logic
	//...
	if(gUseTextureNormal)
	{
	float3 binormal = normalize(cross(normal, tangent));
	
	float3x3 localAxis = float3x3(tangent, binormal, normal);
	
	float3 sampledNormal = (gTextureNormal.Sample(gTextureSampler, texCoord) * 2) -1 ;
	
	newNormal = mul(localAxis, sampledNormal);
	}
	return newNormal;
}

//FRESNEL FALLOFF FUNCTION
float3 CalculateFresnelFalloff(float3 normal, float3 viewDirection, float3 environmentColor)
{
	//Fresnel Falloff Logic
	//...
	if(gUseFresnelFalloff)
	{
	    float Fresnel = pow(1 - saturate(abs(dot(normal,viewDirection))), gFresnelPower) * gFresnelHardness;
	
	    float FresnelMask = pow(1- saturate(float3(0,-1,0)*normal), gFresnelHardness);
		
		if(gUseTextureEnvironment)
	    {
	    return (Fresnel * FresnelMask * environmentColor);
	    }
		else{
		return (Fresnel * FresnelMask * gColorFresnel);
		}
	}
	return environmentColor;
}


//ENVIRONMENT MAPPING FUNCTION
float3 CalculateEnvironment(float3 viewDirection, float3 normal)
{
	float3 environmentColor =  float3(0,0,0);
	
	if(gUseTextureEnvironment)
	{
	float3 reflectedVector = reflect(viewDirection, normal);
	
	float3 refractedVector = refract(viewDirection, normal, gRefractionIndex);
	
	environmentColor += gCubeEnvironment.Sample(gTextureSampler, reflectedVector) * gReflectionStrength;
	environmentColor += gCubeEnvironment.Sample(gTextureSampler, refractedVector) * gRefractionStrength;
	}
	return environmentColor;
}

//OPACITY FUNCTION
float CalculateOpacity(float2 texCoord)
{
	float opacity = gOpacityIntensity;
	
	//Opacity Logic
	//...
	if(gUseTextureOpacity)
	opacity =      gTextureOpacity.Sample(   gTextureSampler,     texCoord).r;
	
	return opacity;
}

//MAIN VERTEX SHADER
//******************
VS_Output MainVS(VS_Input input) {
	
	VS_Output output = (VS_Output)0;
	
	output.Position = mul(float4(input.Position, 1.0), gMatrixWVP);
	output.WorldPosition = mul(float4(input.Position,1.0), gMatrixWorld);
	output.Normal = mul(input.Normal, (float3x3)gMatrixWorld);
	output.Tangent = mul(input.Tangent, (float3x3)gMatrixWorld);
	output.TexCoord = input.TexCoord;
	
	return output;
}

//MAIN PIXEL SHADER
//*****************
float4 MainPS(VS_Output input) : SV_TARGET 
{
	// NORMALIZE
	input.Normal = normalize(input.Normal);
	input.Tangent = normalize(input.Tangent);
	
	float3 viewDirection = normalize(input.WorldPosition.xyz - gMatrixViewInverse[3].xyz);
	
	//NORMAL
	float3 newNormal = CalculateNormal(input.Tangent, input.Normal, input.TexCoord);
		
	//SPECULAR
	float3 specColor = CalculateSpecular(viewDirection, newNormal, input.TexCoord);
		
	//DIFFUSE
	float3 diffColor = CalculateDiffuse(newNormal, input.TexCoord);
		
	//AMBIENT
	float3 ambientColor = gColorAmbient * gAmbientIntensity;
		
	//ENVIRONMENT MAPPING
	float3 environmentColor = CalculateEnvironment(viewDirection, newNormal);
	
	//FRESNEL FALLOFF
	environmentColor = CalculateFresnelFalloff(newNormal, viewDirection, environmentColor);
		
	//FINAL COLOR CALCULATION
	float3 finalColor = diffColor + specColor + environmentColor + ambientColor;
	
	//OPACITY
	float opacity = CalculateOpacity(input.TexCoord);
	
	return float4(finalColor,opacity);
}

// Default Technique
technique10 WithAlphaBlending {
	pass p0 {
		SetRasterizerState(gRS_FrontCulling);
		SetBlendState(gBS_EnableBlending,float4(0.0f, 0.0f, 0.0f, 0.0f), 0xFFFFFFFF);
		SetVertexShader(CompileShader(vs_4_0, MainVS()));
		SetGeometryShader( NULL );
		SetPixelShader(CompileShader(ps_4_0, MainPS()));
	}
}

// Default Technique
technique10 WithoutAlphaBlending {
	pass p0 {
		SetRasterizerState(gRS_FrontCulling);
		SetVertexShader(CompileShader(vs_4_0, MainVS()));
		SetGeometryShader( NULL );
		SetPixelShader(CompileShader(ps_4_0, MainPS()));
	}
}