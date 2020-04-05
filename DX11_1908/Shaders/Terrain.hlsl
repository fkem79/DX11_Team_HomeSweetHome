#include "Header.hlsli"

SamplerComparisonState depthSamp : register(s10);

cbuffer Mapping : register(b10)
{
    int isSpecularMap;
    int isNormalMap;
}

struct PixelInput
{
    float4 position : SV_Position;
    float2 uv : UV;
    float3 tangent : TANGENT;
    float3 binormal : BINORMAL;
    float3 normal : NORMAL;
    float3 viewDir : VIEWDIR;
    float4 vPosition : LIGHTVIEW;
};

PixelInput VS(VertexUVNormalTangent input)
{
    PixelInput output;
    output.position = mul(input.position, world);
    
    output.viewDir = normalize(output.position.xyz - invView._41_42_43);
    
    output.position = mul(output.position, view);
    output.position = mul(output.position, projection);
    
    output.vPosition = mul(input.position, world);
    output.vPosition = mul(output.vPosition, LightV);
    output.vPosition = mul(output.vPosition, LightP);
    
    output.uv = input.uv;
    
    output.normal = mul(input.normal, (float3x3) world);
    output.tangent = mul(input.tangent, (float3x3) world);
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

float4 PS_Depth(PixelInput input) : SV_TARGET
{
    float depth = input.position.z / input.position.w;
    
    return float4(depth.xxx, 1.0f);
}

float4 PS_Shadow(PixelInput input) : SV_TARGET
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
    
    float4 color = diffuse * mDiffuse + specular * mSpecular + ambient * mAmbient;
    
    input.vPosition.xyz /= input.vPosition.w;
    
    if (input.vPosition.x < -1.0f || input.vPosition.x > 1.0f ||
        input.vPosition.y < -1.0f || input.vPosition.y > 1.0f ||
        input.vPosition.z < 0.0f || input.vPosition.z > 1.0f)
        return color;

    input.vPosition.x = input.vPosition.x * 0.5f + 0.5f;
    input.vPosition.y = -input.vPosition.y * 0.5f + 0.5f;
    input.vPosition.z -= bias;
    
    float depth = 0;
    float factor = 0;
    
    [flatten]
    if(quality == 0)
    {
        depth = depthMap.Sample(pointSamp, input.vPosition.xy).r;
        factor = (float) input.vPosition.z <= depth;
    }else if(quality == 1)
    {
        depth = depthMap.SampleCmp(depthSamp, input.vPosition.xy, input.vPosition.z).r;
        factor = (float) input.vPosition.z <= depth;
    }else if(quality == 2)
    {
        float avg = 0;
        float sum = 0;
        
        for (float y = -0.5f; y <= 0.5f; y += 1.0f)
        {
            for (float x = -0.5f; x <= 0.5f; x +=1.0f)
            {
                float2 offset = float2(x / mapSize.x, y / mapSize.y);
                //sum += depthMap.SampleCmpLevelZero(depthSamp, input.vPosition.xy + offset,
                //input.vPosition.z).r;
                sum += depthMap.Sample(pointSamp, input.vPosition.xy + offset).r;
                avg++;
            }
        }
        factor = sum / avg;
    }   
    
    factor = saturate(factor + depth);
    if(factor < 1)
        factor = 1.0f - factor;
    color.rgb *= factor;
    
    return color;
}