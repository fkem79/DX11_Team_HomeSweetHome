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

cbuffer Target : register(b11)
{
    float3 waveLength;
    int sampleCount;

    float3 invWaveLength;
    float targetPadding;

    float3 waveLengthMie;
    float targetPadding2;
}

struct PixelTargetInput
{
    float4 position : SV_POSITION;
    float2 uv : UV0;
};

PixelTargetInput VS(VertexUV input)
{
    PixelTargetInput output;
    output.position = input.position;
    output.uv = input.uv;

    return output;
}

struct PixelTargetOutput
{
    float4 RColor : SV_TARGET0;
    float4 MColor : SV_TARGET1;
};

float HitOuterSphere(float3 position, float3 dir)
{
    float3 light = -position;

    float b = dot(light, dir);
    float c = dot(light, light);

    float d = c - b * b;
    float q = sqrt(OuterRadius * OuterRadius - d);

    float t = b;
    t += q;

    return t;
}

float2 GetDensityRatio(float height)
{
    float altitude = (height - InnerRadius) * Scale;

    return exp(-altitude / RayleighMieScaleHeight);
}

float2 GetDistance(float3 p1, float3 p2)
{
    float2 opticalDepth = 0;

    float3 temp = p2 - p1;
    float far = length(temp);
    float3 dir = temp / far;

    float sampleLength = far / sampleCount;
    float scaledLength = sampleLength * Scale;

    float3 sampleRay = dir * sampleLength;
    p1 += sampleRay * 0.5f;

    for (int i = 0; i < sampleCount; i++)
    {
        float height = length(p1);
        opticalDepth += GetDensityRatio(height);

        p1 += sampleRay;
    }

    return opticalDepth * scaledLength;
}

PixelTargetOutput PS(PixelTargetInput input)
{
    PixelTargetOutput output;

    float3 sunDirection = -normalize(lightDir);
    float2 uv = input.uv;

    float3 pointPv = float3(0, InnerRadius + 1e-3f, 0.0f);
    float angleXZ = PI * uv.y;
    float angleY = 100.0f * uv.x * PI / 180.0f;

    float3 dir;
    dir.x = sin(angleY) * cos(angleXZ);
    dir.y = cos(angleY);
    dir.z = sin(angleY) * sin(angleXZ);
    dir = normalize(dir);

    float farPvPa = HitOuterSphere(pointPv, dir);
    float3 ray = dir;

    float3 pointP = pointPv;
    float sampleLength = farPvPa / sampleCount;
    float scaleLength = sampleLength * Scale;
    float3 sampleRay = ray * sampleLength;
    pointP += sampleRay * 0.5f;

    float3 rayleighSum = 0;
    float3 mieSum;

    for (int i = 0; i < sampleCount; i++)
    {
        float pHeight = length(pointP);

        float2 densityRatio = GetDensityRatio(pHeight);
        densityRatio *= scaleLength;

        float2 viewerOpticalDepth = GetDistance(pointP, pointPv);

        float farPPc = HitOuterSphere(pointP, sunDirection);
        float2 sunOpticalDepth = GetDistance(pointP, pointP + sunDirection * farPPc);

        float2 opticalDepthP = sunOpticalDepth.xy + viewerOpticalDepth.xy;
        float3 attenuation = exp(-Kr4PI * invWaveLength * opticalDepthP.x - Km4PI * opticalDepthP.y);

        rayleighSum += densityRatio.x * attenuation;
        mieSum += densityRatio.y * attenuation;

        pointP += sampleRay;
    }

    float3 rayleigh = rayleighSum * KrESun;
    float3 mie = mieSum * KmESun;

    rayleigh *= invWaveLength;
    mie *= waveLength;

    output.RColor = float4(rayleigh, 1);
    output.MColor = float4(mie, 1);    

    return output;    
}