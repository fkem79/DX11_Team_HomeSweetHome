#include "Header.hlsli"

struct PixelInput
{
    float4 position : SV_Position;    
    float2 uv : UV;
};

PixelInput VS(VertexUV input)
{
    PixelInput output;
    output.position = mul(input.position, world);    
    output.position = mul(output.position, view);
    output.position = mul(output.position, projection);  
    
    output.uv = input.uv;
    
    return output;
}

float4 PS(PixelInput input) : SV_TARGET
{
    return diffuseMap.Sample(linearSamp, input.uv) * mDiffuse;
}