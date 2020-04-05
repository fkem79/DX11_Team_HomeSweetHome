#include "Header.hlsli"

cbuffer Type : register(b11)
{
    int type;
}

cbuffer Mapping : register(b10)
{
    int isSpecularMap;
    int isNormalMap;
}

struct VertexUVNormalTangentBlendMesh
{
    float4 position : POSITION;
    float2 uv : UV;
    float3 normal : NORMAL;
    float3 tangent : TANGENT;
    float4 indices : BLENDINDICES;
    float4 weights : BLENDWEIGHTS;
    
    matrix transform : INSTANCE;
    uint instanceID : SV_InstanceID;
};

struct PixelInput
{
    float4 position : SV_Position;    
};

PixelInput VS(VertexUVNormalTangentBlendMesh input)
{
    PixelInput output;
        
    matrix transform = SkinWorld(input.instanceID, input.indices, input.weights);    
    matrix boneWorld = mul(transform, input.transform);
    
    if(type)
        boneWorld = world;
    
    output.position = mul(input.position, boneWorld);
    output.position = mul(output.position, view);
    output.position = mul(output.position, projection);    
    
    return output;
}

float4 PS(PixelInput input) : SV_TARGET
{
    float depth = input.position.z / input.position.w;
    
    return float4(depth.xxx, 1);
}