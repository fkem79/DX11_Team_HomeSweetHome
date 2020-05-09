#include "Header.hlsli"

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
    float2 uv : UV;
    float3 tangent : TANGENT;
    float3 binormal : BINORMAL;
    float3 normal : NORMAL;
    float3 viewDir : VIEWDIR;
    
    float3 wPosition : POSITION0;
    float3 cPosition : POSITION1;
};

PixelInput VS(VertexUVNormalTangentBlendMesh input)
{
    PixelInput output;

    matrix transform = SkinWorld(input.instanceID, input.indices, input.weights);
    
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
    
    /*float diffuseIntensity = saturate(dot(normal, -light));
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
    
    float4 ambient = albedo * lightAmbient;*/
    
   // return diffuse * mDiffuse + specular * mSpecular + ambient * mAmbient;
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

}

float4 PS_Depth(PixelInput input) : SV_TARGET
{
    float depth = input.position.z / input.position.w;
    
    return float4(depth.xxx, 1.0f);
}