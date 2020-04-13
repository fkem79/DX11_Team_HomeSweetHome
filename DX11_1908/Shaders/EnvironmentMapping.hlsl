#include "Header.hlsli"

TextureCube environmentMap : register(t10);

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
    float3 viewReflect = reflect(input.viewDir, input.normal);    
    
    return environmentMap.Sample(linearSamp, viewReflect);
}