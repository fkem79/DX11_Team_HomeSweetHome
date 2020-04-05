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
};

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
    
    output.viewDir = normalize(output.position.xyz - invView._41_42_43);
    
    output.position = mul(output.position, view);
    output.position = mul(output.position, projection);
    
    output.uv = input.uv;
    
    output.normal = mul(input.normal, (float3x3) boneWorld);
    output.tangent = mul(input.tangent, (float3x3) boneWorld);
    output.binormal = cross(output.normal, output.tangent);
    
    return output;
}

float4 PS(PixelInput input) : SV_TARGET
{
    float4 albedo = diffuseMap.Sample(linearSamp, input.uv);
    
    float3 light = normalize(lightDir);
    
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
    
    float diffuseIntensity = saturate(dot(normal, -light));
    float4 diffuse = albedo * diffuseIntensity;
    
    float4 specular = 0;
    if (diffuseIntensity > 0)
    {
        float3 halfWay = normalize(input.viewDir + light);
        specular = saturate(dot(-halfWay, normal));
        
        float4 specularIntensity = 1;
        if (isSpecularMap)
        {
            specularIntensity = specularMap.Sample(linearSamp, input.uv);
        }
        
        specular = pow(specular, lightSpecExp) * specularIntensity;
    }
    
    float4 ambient = albedo * lightAmbient;
    
    return diffuse * mDiffuse + specular * mSpecular + ambient * mAmbient;
}