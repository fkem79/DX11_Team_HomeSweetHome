#include "Header.hlsli"

cbuffer Color : register(b10)
{
    float4 color;
}

struct PixelInput
{
    float4 position : SV_Position;    
};

PixelInput VS(Vertex input)
{
    PixelInput output;
    output.position = mul(input.position, world);
    output.position = mul(output.position, view);
    output.position = mul(output.position, projection);
    
    return output;
}

float4 PS(PixelInput input) : SV_TARGET
{
    return color;
}