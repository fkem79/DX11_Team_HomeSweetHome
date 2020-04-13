#include "Header.hlsli"

Texture2D reflectionMap : register(t10);
Texture2D refractionMap : register(t11);

cbuffer Water : register(b10)
{    
    float4 refractionColor;
    
    float waveTranslation;
    float waveScale;
    float waterShininess;
    float waterAlpha;
    
    float waveSpeed;
}

cbuffer Reflection : register(b10)
{
    matrix reflection;
}

struct PixelInput
{
    float4 position : SV_Position;
    float3 wPosition : POSITION1;
    float4 reflectionPosition : POSITION2;
    float4 refractionPosition : POSITION3;
    
    float2 uv : UV0;    
};

PixelInput VS(VertexUV input)
{
    PixelInput output;
    
    output.position = mul(input.position, world);
    output.wPosition = output.position.xyz;    
    
    output.position = mul(output.position, view);
    output.position = mul(output.position, projection);
    
    output.uv = input.uv;
    
    output.reflectionPosition = mul(input.position, world);
    output.reflectionPosition = mul(output.reflectionPosition, reflection);
    output.reflectionPosition = mul(output.reflectionPosition, projection);
    
    output.refractionPosition = output.position;
    
    return output;
}

float4 PS(PixelInput input) : SV_TARGET
{
    input.uv += waveTranslation;
    
    float4 normalMapping = normalMap.Sample(linearSamp, input.uv) * 2.0f - 1.0f;
    float3 normal = normalMapping.rgb;
    
    float2 reflection;
    reflection.x = input.reflectionPosition.x / input.reflectionPosition.w * 0.5f + 0.5f;
    reflection.y = -input.reflectionPosition.y / input.reflectionPosition.w * 0.5f + 0.5f;
    reflection += normal.xy * waveScale;
    float4 reflectionColor = reflectionMap.Sample(linearSamp, reflection);
    
    float2 refraction;
    refraction.x = input.refractionPosition.x / input.refractionPosition.w * 0.5f + 0.5f;
    refraction.y = -input.refractionPosition.y / input.refractionPosition.w * 0.5f + 0.5f;
    refraction += normal.xy * waveScale;
    float4 refractColor = saturate(refractionMap.Sample(linearSamp, refraction) + refractionColor);    
 
    //float3 light = lightDir;
    //light.y *= -1.0f;
    //light.z *= -1.0f;
    
    float3 view = normalize(CamPos() - input.wPosition);
    float3 heightView = view.yyy;

    float r = (1.2f - 1.0f) / (1.2f / 1.0f);
    float fresnel = saturate(min(1, r + (1 - r) * pow(1 - dot(normal, heightView), 2)));
    float4 diffuse = lerp(reflectionColor, refractColor, fresnel);    
    
    //float3 halfWay = normalize(view + light);
    //float3 halfWay = normalize(lightDir);
    //float specularIntensity = saturate(dot(halfWay, normal));
    
    /*
    [flatten]
    if(specular > 0.0f)
    {
        specular = pow(specular, waterShininess);
        diffuse = saturate(diffuse + specular);
    }
    
    return float4(diffuse.rgb, waterAlpha);
*/
    return diffuse;

}