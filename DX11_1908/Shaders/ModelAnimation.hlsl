#include "Header.hlsli"

cbuffer Frame : register(b10)
{
    TweenFrame tween;
}

cbuffer Mapping : register(b10)
{
    int isSpecularMap;
    int isNormalMap;
}

//Texture2DArray transformMap : register(t10);

struct PixelInput
{
    float4 position : SV_Position;
    float2 uv : UV;
    float3 tangent : TANGENT;
    float3 binormal : BINORMAL;
    float3 normal : NORMAL;
    float3 viewDir : VIEWDIR;
};

PixelInput VS(VertexUVNormalTangentBlend input)
{
    PixelInput output;

    matrix transform = 0;
    matrix cur = 0, curAnim = 0;
    matrix next = 0, nextAnim = 0;
    
    float indices[4] = { input.indices.x, input.indices.y, input.indices.z, input.indices.w };
    float weights[4] = { input.weights.x, input.weights.y, input.weights.z, input.weights.w };
    
    int clip[2];
    uint curFrame[2];
    uint nextFrame[2];
    float time[2];
    
    clip[0] = tween.cur.clip;
    curFrame[0] = tween.cur.curFrame;
    nextFrame[0] = tween.cur.nextFrame;
    time[0] = tween.cur.time;
    
    clip[1] = tween.next.clip;
    curFrame[1] = tween.next.curFrame;
    nextFrame[1] = tween.next.nextFrame;
    time[1] = tween.next.time;
    
    float4 c0, c1, c2, c3;
    float4 n0, n1, n2, n3;
    
    [unroll(4)]
    for (int i = 0; i < 4; i ++)
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
        
        curAnim = lerp(cur, next, time[0]);
        
        [flatten]
        if(clip[1] > -1)
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
            
            curAnim = lerp(curAnim, nextAnim, tween.tweenTime);
        }
        
        transform += mul(weights[i], curAnim);
    }
    
    matrix boneWorld = mul(transform, world);
    
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