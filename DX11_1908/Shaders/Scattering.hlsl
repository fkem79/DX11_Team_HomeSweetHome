#include "Header.hlsli"

static const float PI = 3.14159265f;
static const float InnerRadius = 6356.7523142f;
static const float OuterRadius = 6356.7523142f * 1.0157313f;

static const float KrESun = 0.0025 * 20.0f; //0.0025f - 레일리 상수 * 태양밝기
static const float KmESun = 0.0010 * 20.0f; //0.0010f - 미 상수 * 태양밝기
static const float Kr4PI = 0.0025 * 4 * 3.1415159;
static const float Km4PI = 0.0010 * 4 * 3.1415159;

static const float2 RayleighMieScaleHeight = { 0.25f, 0.1f };
static const float Scale = 1.0f / (6356.7523142 * 1.0157313 - 6356.7523142);

static const float g = -0.990f;
static const float g2 = -0.990f * -0.990f;
static const float Exposure = -2.0f;

struct PixelInput
{
    float4 position : SV_POSITION;
    float2 uv : UV0;
    float3 oPosition : POSITION0;
};

PixelInput VS(VertexUV input)
{
    PixelInput output;

    output.position = mul(input.position, world);
    output.position = mul(output.position, view);
    output.position = mul(output.position, projection);

    output.oPosition = -input.position;
    output.uv = input.uv;

    return output;
}

cbuffer PS_Scatter : register(b10)
{
    float starIntetnsity;
}

Texture2D rayleighMap : register(t10);
Texture2D mieMap : register(t11);
Texture2D starMap : register(t12);

float GetRayleighPhase(float c)
{
    return 0.75f * (1.0f + c);
}

float GetMiePhase(float c, float c2)
{
    float3 result = 0;
    result.x = 1.5f * ((1.0f - g2) / (2.0f + g2));
    result.y = 1.0f + g2;
    result.z = 2.0f * g;

    return result.x * (1.0f + c2) / pow(result.y - result.z * c, 1.5f);
}

float3 HDR(float3 LDR)
{
    return 1.0f - exp(Exposure * LDR);
}

float4 PS(PixelInput input) : SV_Target
{
    float3 sunDirection = -normalize(lightDir);

    float temp = dot(sunDirection, input.oPosition) / length(input.oPosition);
    float temp2 = temp * temp;

    float3 rSamples = rayleighMap.Sample(linearSamp, input.uv);
    float3 mSamples = mieMap.Sample(linearSamp, input.uv);

    float3 color = 0;
    color = GetRayleighPhase(temp2) * rSamples + GetMiePhase(temp, temp2) * mSamples;
    color = HDR(color);

    color += max(0, (1 - color.rgb)) * float3(0.05f, 0.05f, 0.1f);
    
    float intensity = saturate(starIntetnsity);
    return float4(color, 1) + starMap.Sample(linearSamp, input.uv) * intensity;
}