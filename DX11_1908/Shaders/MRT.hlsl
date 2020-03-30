#include "Header.hlsli"

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

struct PixelOutput
{
    float4 color1 : SV_TARGET0;
    float4 color2 : SV_TARGET1;
};

PixelOutput PS(PixelInput input)
{
    PixelOutput output;
    
    float4 color = map.Sample(pointSamp, input.uv);
    
    output.color1 = float4(1.0f - color.rgb, 1.0f);
    
    float grayScale = (color.r + color.g + color.b) / 3.0f;    
    output.color2 = float4(grayScale.xxx, 1.0f);
    
    return output;
}