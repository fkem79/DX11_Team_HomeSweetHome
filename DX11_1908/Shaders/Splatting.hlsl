//Splatting.hlsl
#include "Header.hlsli"

cbuffer VS_Brush : register(b10)
{
    int type;
    float3 location;
    
    float range;
    float3 color;
}

Texture2D secondMap : register(t10);
Texture2D thirdMap : register(t11);

struct VertexAlpha
{
    float4 position : POSITION;
    float2 uv : UV;
    float3 normal : NORMAL;
    float4 alpha : ALPHA;
};

struct PixelInput
{
    float4 position : SV_Position;
    float2 uv : UV;
    float3 normal : NORMAL;
    float3 viewDir : VIEWDIR;
    float4 brushColor : COLOR;
    float4 alpha : ALPHA;
};

float3 BrushColor(float3 pos)
{
    if (type == 1)
    {
        float x = pos.x - location.x;
        float z = pos.z - location.z;
        
        float dist = sqrt(x * x + z * z);
        
        if (dist <= range)
            return color;
    }
    
    return float3(0, 0, 0);
}

PixelInput VS(VertexAlpha input)
{
    PixelInput output;
    output.position = mul(input.position, world);
    
    output.viewDir = normalize(output.position.xyz - invView._41_42_43);
    
    output.brushColor = float4(BrushColor(output.position.xyz), 1.0f);
    
    output.position = mul(output.position, view);
    output.position = mul(output.position, projection);
    
    output.uv = input.uv;
    output.alpha = input.alpha;
    
    output.normal = mul(input.normal, (float3x3) world);
    
    return output;
}

float4 PS(PixelInput input) : SV_TARGET
{
    float4 albedo = diffuseMap.Sample(linearSamp, input.uv);    
    float4 second = secondMap.Sample(linearSamp, input.uv);
    float4 third = thirdMap.Sample(linearSamp, input.uv);
    
    albedo = lerp(albedo, second, input.alpha.r);
    albedo = lerp(albedo, third, input.alpha.g);
    
    float3 light = normalize(lightDir);
    
    float3 normal = normalize(input.normal);
    
    float diffuseIntensity = saturate(dot(normal, -light));     
    float4 diffuse = albedo * diffuseIntensity;
    
    float4 specular = 0;
    if (diffuseIntensity > 0)
    {
        float3 halfWay = normalize(input.viewDir + light);
        specular = saturate(dot(-halfWay, normal));
        
        float4 specularIntensity = 1;
        
        specular = pow(specular, lightSpecExp) * specularIntensity;
    }
    
    float4 ambient = albedo * lightAmbient;
    
    return diffuse * mDiffuse + specular * mSpecular + ambient * mAmbient + input.brushColor;
}