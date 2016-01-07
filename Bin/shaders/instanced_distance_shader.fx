#include "common.fx"

Texture2D shaderTexture;
SamplerState SampleType;

struct VertexInputType
{
    float4 position : POSITION;
    float2 tex : TEXCOORD0;
	float2 instancePosition : TEXCOORD1;
	float4 instanceColor : TEXCOORD2;
	float4 instanceRotationAndSize : TEXCOORD3;
	float4 instanceUV : TEXCOORD4;
	float4 instancePivot : TEXCOORD5;
};

struct PixelInputType
{
    float4 position : SV_POSITION;
    float2 tex : TEXCOORD0;
	float4 color : TEXCOORD1;
	float distanceDelta : TEXCOORD2;
};

float2x2 ComputeParticleRotation(float aRotation)
{ 
	float c = cos(aRotation);
	float s = sin(aRotation);
	return float2x2(c, -s, s, c);
}

PixelInputType VShader(VertexInputType input)
{
    PixelInputType output;

    // Change the position vector to be 4 units for proper matrix calculations.
    input.position.w = 1.0f;
	
	input.position.x -= input.instancePivot.x;
	input.position.y += input.instancePivot.y;
	    
	input.position.xy *= input.instanceRotationAndSize.yz;
	
	float2x2 theRotation = ComputeParticleRotation(input.instanceRotationAndSize.x);
	input.position.xy = mul(input.position.xy, theRotation);	
	
	input.position.x += input.instancePosition.x;
    input.position.y += input.instancePosition.y;
	
    output.position = mul(input.position, worldMatrix);
    output.position = mul(output.position, viewMatrix);
    output.position = mul(output.position, projectionMatrix);
	    
	output.tex.x = input.instanceUV.x + (input.tex.x * input.instanceUV.z);
	output.tex.y = input.instanceUV.y + (input.tex.y * input.instanceUV.w);
	output.color = input.instanceColor;
	output.distanceDelta = min(input.instanceRotationAndSize.w, 0.1);
	
    return output;
}

float4 PShader(PixelInputType input) : SV_TARGET
{
	float delta = 0.3f; 
	float rawAlpha = shaderTexture.Sample( SampleType, input.tex).r;
	float4 finalColor = float4(input.color.xyz, smoothstep(0.5f-delta,0.5f+delta,rawAlpha) );
	finalColor.a *= input.color.a;
	
	return finalColor;      

	
}
