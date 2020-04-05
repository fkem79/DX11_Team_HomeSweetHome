#include "Header.hlsli"

Texture2D reflectionMap : register(t10);

cbuffer Reflection : register(b10)
{
    matrix reflection;
}

struct PixelInput
{
    float4 position : SV_Position;
    float2 uv : UV;
    float3 normal : NORMAL;
    float3 viewDir : VIEWDIR;
    
    float4 wvpPositionSub : POSITION;
};

PixelInput VS(VertexUVNormal input)
{
    PixelInput output;
    
    output.position = mul(input.position, world);
    
    output.viewDir = normalize(output.position.xyz - CamPos());
    
    output.wvpPositionSub = mul(output.position, reflection);
    output.wvpPositionSub = mul(output.wvpPositionSub, projection);
    
    output.position = mul(output.position, view);
    output.position = mul(output.position, projection);
    
    output.uv = input.uv;
    
    output.normal = mul(input.normal, (float3x3) world);
    
    return output;
}

float4 PS(PixelInput input) : SV_TARGET
{
    float4 refPosition = input.wvpPositionSub;
    
    float2 reflection;
    reflection.x = refPosition.x / refPosition.w * 0.5f + 0.5f;
    reflection.y = -refPosition.y / refPosition.w * 0.5f + 0.5f;
    
    return reflectionMap.Sample(linearSamp, reflection);
}