#include "Header.hlsli"

cbuffer BlurBuffer : register(b10)
{
    int count;
    float radius;
    float amount;
}

Texture2D map : register(t10);

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
    float2 radiusUV = input.uv - float2(0.5f, 0.5f);
    float r = length(radiusUV);
    radiusUV /= r;
    
    r = saturate(2 * r / radius);
    
    float2 delta = -radiusUV * r * r * amount / count;
    
    float4 color = 0;
    
    for (int i = 0; i < count; i++)
    {
        color += map.Sample(pointSamp, input.uv);
        input.uv += delta;
    }
    
    color /= count;
    
    return color;
}