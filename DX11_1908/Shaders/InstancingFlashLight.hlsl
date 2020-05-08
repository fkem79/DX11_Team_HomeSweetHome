#include "Header.hlsli"

cbuffer Mapping : register(b10)
{
    int isSpecularMap;
    int isNormalMap;
}

struct VertexInput
{
    float4 position : POSITION;
    float2 uv : UV;
    float3 normal : NORMAL;
    float3 tangent : TANGENT;
    
    matrix transform : INSTANCE;
    uint instanceID : SV_InstanceID;
};

struct PixelInput
{
    float4 position : SV_Position;
    float2 uv : UV;
    float3 tangent : TANGENT;
    float3 binormal : BINORMAL;
    float3 normal : NORMAL;
    float3 viewDir : VIEWDIR;
    
    float3 wPosition : POSITION0;
    float3 cPosition : POSITION1;
};

/*
struct VertexInput
{
    float4 position : POSITION;
    float2 uv : UV;
    float3 normal : NORMAL;
    float3 tangent : TANGENT;
    
    matrix transform : INSTANCE;
    uint instanceID : SV_InstanceID;
};

struct PixelInput
{
    float4 position : SV_Position;
    float2 uv : UV;    
    float3 normal : NORMAL;
    float3 wPosition : POSITION0;
    float3 cPosition : POSITION1;
};
*/
//Texture2DArray transformMap : register(t10);

PixelInput VS(VertexInput input)
{
    PixelInput output;

    //matrix boneWorld = mul(bones[index], world);
    
    float4 m0 = transformMap.Load(int4(index * 4 + 0, input.instanceID, 0, 0));
    float4 m1 = transformMap.Load(int4(index * 4 + 1, input.instanceID, 0, 0));
    float4 m2 = transformMap.Load(int4(index * 4 + 2, input.instanceID, 0, 0));
    float4 m3 = transformMap.Load(int4(index * 4 + 3, input.instanceID, 0, 0));
    
    matrix transform = matrix(m0, m1, m2, m3);
    matrix boneWorld = mul(transform, input.transform);
    
    output.position = mul(input.position, boneWorld);
    output.wPosition = output.position;
    
    output.viewDir = normalize(output.position.xyz - invView._41_42_43);
    
    output.position = mul(output.position, view);
    output.position = mul(output.position, projection);
    
    output.uv = input.uv;
    
    output.normal = mul(input.normal, (float3x3) boneWorld);
    output.tangent = mul(input.tangent, (float3x3) boneWorld);
    output.binormal = cross(output.normal, output.tangent);
    
    output.cPosition = CamPos();
    
    return output;
}


float4 PS(PixelInput input) : SV_TARGET
{
    
    float4 albedo = diffuseMap.Sample(linearSamp, input.uv) * mDiffuse;
    
    //float3 light = normalize(lightDir);
    
    float3 T = normalize(input.tangent);
    float3 B = normalize(input.binormal);
    float3 N = normalize(input.normal);
    
    float3 normal = N;
    if (isNormalMap)
    {
        float4 normalMapping = normalMap.Sample(linearSamp, input.uv);
    
        float3x3 TBN = float3x3(T, B, N);
    
        normal = normalMapping.xyz * 2.0f - 1.0f;
        normal = normalize(mul(normal, TBN));
    }
    
    float4 result = CalcAmbient(normal, albedo);
        
    for (uint i = 0; i < lightCount; i++)
    {
        [flatten]
        if (lights[i].type == 0)
        {
            result += CalcDirection(normal, albedo, input.wPosition, input.cPosition, lights[i]);
        }
        else if (lights[i].type == 1)
        {
            result += CalcPoint(normal, albedo, input.wPosition, input.cPosition, lights[i]);
        }
        else if (lights[i].type == 2)
        {
            result += CalcSpot(normal, albedo, input.wPosition, input.cPosition, lights[i]);
        }
        else if (lights[i].type == 3)
        {
            result += CalcCapsule(normal, albedo, input.wPosition, input.cPosition, lights[i]);
        }
    }
    
    return result/* + ambient * mAmbient*/;
    //return diffuse * mDiffuse + specular * mSpecular + ambient * mAmbient * result;
}
/*
float4 PS(PixelInput input) : SV_TARGET
{
    float4 albedo = diffuseMap.Sample(linearSamp, input.uv);
    float3 normal = normalize(input.normal);
    
    float4 result = CalcAmbient(normal, albedo);
        
    for (uint i = 0; i < lightCount; i++)
    {
        [flatten]
        if (lights[i].type == 0)
        {
            result += CalcDirection(normal, albedo, input.wPosition, input.cPosition, lights[i]);
        }
        else if (lights[i].type == 1)
        {
            result += CalcPoint(normal, albedo, input.wPosition, input.cPosition, lights[i]);
        }
        else if (lights[i].type == 2)
        {
            result += CalcSpot(normal, albedo, input.wPosition, input.cPosition, lights[i]);
        }
        else if (lights[i].type == 3)
        {
            result += CalcCapsule(normal, albedo, input.wPosition, input.cPosition, lights[i]);
        }
    }
    
    return result;
}*/