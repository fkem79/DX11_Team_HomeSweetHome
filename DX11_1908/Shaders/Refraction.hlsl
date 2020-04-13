#include "Header.hlsli"

Texture2D refractionMap : register(t10);

cbuffer Time : register(b10)
{
    float time;
}

struct PixelInput
{
    float4 position : SV_Position;
    float2 uv : UV;
    float3 normal : NORMAL;
    float3 viewDir : VIEWDIR;    
};

PixelInput VS(VertexUVNormal input)
{
    PixelInput output;
    
    output.position = mul(input.position, world);
    
    output.viewDir = normalize(output.position.xyz - CamPos());
    
    output.position = mul(output.position, view);
    output.position = mul(output.position, projection);
    
    output.uv = input.uv;
    
    output.normal = mul(input.normal, (float3x3) world);
    
    return output;
}

float4 PS(PixelInput input) : SV_TARGET
{
    float4 refPosition = input.position;
    
    float2 refraction;
    refraction.x = refPosition.x / refPosition.w * 0.5f + 0.5f;
    refraction.y = -refPosition.y / refPosition.w * 0.5f + 0.5f;
    
    refraction += refract(input.viewDir, normalize(input.normal), 0.15f).xy;
    
    input.uv.x += (time * 0.25f);
    float3 normal = normalMap.Sample(linearSamp, input.uv).rgb * 2.0f - 1.0f;
    refraction += normal.xy * 0.05f;
    
    return refractionMap.Sample(linearSamp, refraction);
}