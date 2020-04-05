#include "Header.hlsli"

TextureCube cubeMap : register(t10);

struct PixelInput
{
    float4 position : SV_Position;
    float3 oPosition : POSITION;
};

PixelInput VS(VertexUV input)
{
    PixelInput output;
    output.position.xyz = mul(input.position.xyz, (float3x3) view);   
    output.position.w = 1.0f;
    //outtput.position = mul(output.position, view);
    output.position = mul(output.position, projection);
    
    output.oPosition = input.position.xyz;
    
    return output;
}

float4 PS(PixelInput input) : SV_TARGET
{
    return cubeMap.Sample(linearSamp, input.oPosition);
}