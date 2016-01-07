#include "common_sprite.fx"

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

PixelInputType VShader(VertexInputType input)
{
    PixelInputType output;
	
	float2 Resolution = myResolution.xy;
	float ratio = Resolution.y / Resolution.x;

    // Change the position vector to be 4 units for proper matrix calculations.
    input.position.w = 1.0f;
		
	input.position.x -= input.instancePivot.x;
	input.position.y += input.instancePivot.y;
	
	 // Update the position of the vertices based on the data for this particular instance.
    
	input.position.xy *= input.instanceRotationAndSize.yz;
	

	float2x2 theRotation = ComputeParticleRotation(input.instanceRotationAndSize.x);
	input.position.xy = mul(input.position.xy, theRotation);

	input.position.x *= ratio;	
	
	input.position.x += input.instancePosition.x;
    input.position.y += input.instancePosition.y;
	
	output.position = input.position;
    //output.position = mul(input.position, worldMatrix);
    //output.position = mul(output.position, viewMatrix);
    //output.position = mul(output.position, projectionMatrix);
    
	output.tex = input.instanceUV.xy + (input.tex * input.instanceUV.z);
	
	output.color = input.instanceColor;	
	output.textureMappingData = input.instanceRotationAndSize.w;

    return output;
}