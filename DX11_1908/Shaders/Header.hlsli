struct KeyFrame
{
    int clip;
    
    uint curFrame;
    uint nextFrame;
    
    float time;
    float runningTime;
    
    float speed;
    
    float2 padding;
};

struct TweenFrame
{
    float takeTime;
    float tweenTime;
    float runningTime;
    float padding;
    
    KeyFrame cur;
    KeyFrame next;
};

cbuffer VP : register(b0)
{
    matrix view;
    matrix projection;
    matrix invView;
}

cbuffer W : register(b1)
{
    matrix world;
}

cbuffer Bones : register(b2)
{
    matrix bones[300];
    
    int index;
}

cbuffer Frame : register(b3)
{
    TweenFrame tweenFrame[500];
}

cbuffer LightVP : register(b4)
{
    matrix LightV;
    matrix LightP;
}

cbuffer Light : register(b0)
{
    float3 lightDir;    
    float lightSpecExp;    
    float4 lightAmbient;
    float4 lightAmbientFloor;
}

cbuffer Material : register(b1)
{
    float4 mDiffuse;
    float4 mSpecular;
    float4 mAmbient;
}

cbuffer Shadow : register(b2)
{   
    float2 mapSize;
    
    float bias;
    int quality;
}

struct Light
{   
    float4 color;
    
    float3 position;
    float range;
    
    float3 direction;    
    float outer;
    
    float inner;    
    float length;    
    int type;    
    float padding;
};
#define MAX_LIGHT 10

cbuffer LightInfo : register(b3)
{
    Light lights[MAX_LIGHT];
    uint lightCount;
}

Texture2D diffuseMap : register(t0);
Texture2D specularMap : register(t1);
Texture2D normalMap : register(t2);

Texture2DArray transformMap : register(t3);

Texture2D depthMap : register(t4);

SamplerState linearSamp : register(s0);
SamplerState pointSamp : register(s1);

struct Vertex
{
    float4 position : POSITION;
};

struct VertexSize
{
    float4 position : POSITION;
    float2 size : SIZE;
};

struct VertexUV
{
    float4 position : POSITION;
    float2 uv : UV;
};

struct VertexUVNormal
{
    float4 position : POSITION;
    float2 uv : UV;
    float3 normal : NORMAL;
};

struct VertexUVNormalTangent
{
    float4 position : POSITION;
    float2 uv : UV;
    float3 normal : NORMAL;
    float3 tangent : TANGENT;
};

struct VertexUVNormalTangentBlend
{
    float4 position : POSITION;
    float2 uv : UV;
    float3 normal : NORMAL;
    float3 tangent : TANGENT;
    float4 indices : BLENDINDICES;
    float4 weights : BLENDWEIGHTS;
};

float3 CamPos()
{
    return invView._41_42_43;
}

matrix SkinWorld(int instanceID, float4 indices, float4 weights)
{
    matrix transform = 0;
    matrix cur = 0, curAnim = 0;
    matrix next = 0, nextAnim = 0;
    
    [flatten]
    if (any(indices) == false)
    {
        indices[0] = index;
        weights[0] = 1.0f;
    }
    
    int clip[2];
    uint curFrame[2];
    uint nextFrame[2];
    float time[2];
    
    clip[0] = tweenFrame[instanceID].cur.clip;
    curFrame[0] = tweenFrame[instanceID].cur.curFrame;
    nextFrame[0] = tweenFrame[instanceID].cur.nextFrame;
    time[0] = tweenFrame[instanceID].cur.time;
    
    clip[1] = tweenFrame[instanceID].next.clip;
    curFrame[1] = tweenFrame[instanceID].next.curFrame;
    nextFrame[1] = tweenFrame[instanceID].next.nextFrame;
    time[1] = tweenFrame[instanceID].next.time;
    
    float4 c0, c1, c2, c3;
    float4 n0, n1, n2, n3;
    
    [unroll(4)]
    for (int i = 0; i < 4; i++)
    {
        c0 = transformMap.Load(int4(indices[i] * 4 + 0, curFrame[0], clip[0], 0));
        c1 = transformMap.Load(int4(indices[i] * 4 + 1, curFrame[0], clip[0], 0));
        c2 = transformMap.Load(int4(indices[i] * 4 + 2, curFrame[0], clip[0], 0));
        c3 = transformMap.Load(int4(indices[i] * 4 + 3, curFrame[0], clip[0], 0));
        cur = matrix(c0, c1, c2, c3);
        
        n0 = transformMap.Load(int4(indices[i] * 4 + 0, nextFrame[0], clip[0], 0));
        n1 = transformMap.Load(int4(indices[i] * 4 + 1, nextFrame[0], clip[0], 0));
        n2 = transformMap.Load(int4(indices[i] * 4 + 2, nextFrame[0], clip[0], 0));
        n3 = transformMap.Load(int4(indices[i] * 4 + 3, nextFrame[0], clip[0], 0));
        next = matrix(n0, n1, n2, n3);
        
        curAnim = lerp(cur, next, saturate(time[0]));
        
        [flatten]
        if (clip[1] > -1)
        {
            c0 = transformMap.Load(int4(indices[i] * 4 + 0, curFrame[1], clip[1], 0));
            c1 = transformMap.Load(int4(indices[i] * 4 + 1, curFrame[1], clip[1], 0));
            c2 = transformMap.Load(int4(indices[i] * 4 + 2, curFrame[1], clip[1], 0));
            c3 = transformMap.Load(int4(indices[i] * 4 + 3, curFrame[1], clip[1], 0));
            cur = matrix(c0, c1, c2, c3);
        
            n0 = transformMap.Load(int4(indices[i] * 4 + 0, nextFrame[1], clip[1], 0));
            n1 = transformMap.Load(int4(indices[i] * 4 + 1, nextFrame[1], clip[1], 0));
            n2 = transformMap.Load(int4(indices[i] * 4 + 2, nextFrame[1], clip[1], 0));
            n3 = transformMap.Load(int4(indices[i] * 4 + 3, nextFrame[1], clip[1], 0));
            next = matrix(n0, n1, n2, n3);
        
            nextAnim = lerp(cur, next, time[1]);
            
            curAnim = lerp(curAnim, nextAnim, tweenFrame[instanceID].tweenTime);
        }
        
        transform += mul(weights[i], curAnim);
    }
    
    return transform;
}

float4 CalcAmbient(float3 normal, float4 color)
{
    float up = normal.y * 0.5f + 0.5f;
    
    float4 ambient = lightAmbientFloor + up * lightAmbient;
    
    return ambient * color;
}

float4 CalcDirection(float3 normal, float4 color, float3 wPosition, float3 cPosition, Light light)
{
    float3 toLight = -normalize(light.direction);    
    
    float diffuseIntensity = dot(toLight, normal);
    color *= light.color * saturate(diffuseIntensity) * mDiffuse;
         
    if (diffuseIntensity > 0.0f)
    {
        float3 toEye = normalize(cPosition - wPosition);
        float3 halfWay = normalize(toEye + toLight);
        float specularIntensity = saturate(dot(halfWay, normal));
        color += light.color * pow(specularIntensity, lightSpecExp) * mSpecular;
    }    
    
    return color;
}

float4 CalcPoint(float3 normal, float4 color, float3 wPosition, float3 cPosition, Light light)
{
    float3 toLight = light.position - wPosition;
    float distanceToLight = length(toLight);
    toLight /= distanceToLight;
    
    float diffuseIntensity = dot(toLight, normal);
    color *= light.color * saturate(diffuseIntensity) * mDiffuse;
         
    if (diffuseIntensity > 0.0f)
    {
        float3 toEye = normalize(cPosition - wPosition);
        float3 halfWay = normalize(toEye + toLight);
        float specularIntensity = saturate(dot(halfWay, normal));
        color += light.color * pow(specularIntensity, lightSpecExp) * mSpecular;
    }
    
    //Attenuation
    float distanceToLightNormal = 1.0f - saturate(distanceToLight / light.range);
    float attention = distanceToLightNormal * distanceToLightNormal;
    
    return color * attention;
}

float4 CalcSpot(float3 normal, float4 color, float3 wPosition, float3 cPosition, Light light)
{
    float3 toLight = light.position - wPosition;
    float distanceToLight = length(toLight);
    toLight /= distanceToLight;
    
    float diffuseIntensity = dot(toLight, normal);
    color *= light.color * saturate(diffuseIntensity) * mDiffuse;
         
    if (diffuseIntensity > 0.0f)
    {
        float3 toEye = normalize(cPosition - wPosition);
        float3 halfWay = normalize(toEye + toLight);
        float specularIntensity = saturate(dot(halfWay, normal));
        color += light.color * pow(specularIntensity, lightSpecExp) * mSpecular;
    }
    
    //Attenuation
    float3 dir = -normalize(light.direction);
    float cosAngle = dot(dir, toLight);
    
    float outer = cos(radians(light.outer));
    float inner = 1.0f / cos(radians(light.inner));
    
    float conAttention = saturate((cosAngle - outer) * inner);
    conAttention *= conAttention;
    
    float distanceToLightNormal = 1.0f - saturate(distanceToLight / light.range);
    float attention = distanceToLightNormal * distanceToLightNormal;
    
    return color * attention * conAttention * 8.0f;
}

float4 CalcCapsule(float3 normal, float4 color, float3 wPosition, float3 cPosition, Light light)
{
    float3 direction = normalize(light.direction);
    float3 start = wPosition - light.position;
    float distanceOnLine = dot(start, direction) / light.length;
    distanceOnLine = saturate(distanceOnLine) * light.length;
    
    float3 pointOnLine = light.position + direction * distanceOnLine;
    float3 toLight = pointOnLine - wPosition;
    float distanceToLight = length(toLight);
    toLight /= distanceToLight;
    
    float diffuseIntensity = dot(toLight, normal);
    color *= light.color * saturate(diffuseIntensity) * mDiffuse;
         
    if (diffuseIntensity > 0.0f)
    {
        float3 toEye = normalize(cPosition - wPosition);
        float3 halfWay = normalize(toEye + toLight);
        float specularIntensity = saturate(dot(halfWay, normal));
        color += light.color * pow(specularIntensity, lightSpecExp) * mSpecular;
    }
    
    //Attenuation
    float distanceToLightNormal = 1.0f - saturate(distanceToLight / light.range);
    float attention = distanceToLightNormal * distanceToLightNormal;
    
    return color * attention;
}