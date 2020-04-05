#include "Header.hlsli"

cbuffer Data : register(b10)
{
    float4 color;
    
    float3 velocity;
    float drawDistance;
    
    float3 origin;
    float turbulence;
    
    float3 extent;
    float time;
}

Texture2D map : register(t10);

struct VertexInput
{
    float4 position : POSITION;
    float scale : SCALE;
    float2 random : RANDOM;
};

struct VertexOutput
{
    float3 position : POSITION;
    float2 size : SIZE;    
    float4 color : COLOR;
    float3 velocity : VELOCITY;
    float distance : DISTANCE;
};

VertexOutput VS(VertexInput input)
{
    VertexOutput output;
    
    float3 displace = time * velocity;
    
    input.position.y = origin.y + extent.y - (input.position.y - displace) % extent.y;    
    
    input.position.x += cos(time - input.random.x) * turbulence;
    input.position.z += cos(time - input.random.y) * turbulence;
    
    input.position.xyz = origin + (extent + (input.position.xyz + displace) %
    extent) % extent - (extent * 0.5f); 
    
    output.velocity = velocity;
    output.size = float2(input.scale, input.scale);
    output.color = color;
    
    output.position = input.position.xyz;
    output.distance = drawDistance;
    
    return output;
}

struct PixelInput
{
    float4 position : SV_Position;
    float2 uv : UV;
    float4 color : COLOR;
    float alpha : ALPHA;
};

static const float2 TEXCOORD[4] =
{
    float2(0.0f, 1.0f),
    float2(0.0f, 0.0f),
    float2(1.0f, 1.0f),
    float2(1.0f, 0.0f)
};

[maxvertexcount(4)]
void GS(point VertexOutput input[1], inout TriangleStream<PixelInput> stream)
{
    float3 up = normalize(-input[0].velocity);
    float3 forward = CamPos() - input[0].position;
    forward = normalize(forward);
    
    float3 right = normalize(cross(up, forward));
    
    float halfWidth = input[0].size.x * 0.5f;
    float halfHeight = input[0].size.y * 0.5f;
    
    float4 vertices[4];
    vertices[0] = float4(input[0].position + halfWidth * right - halfHeight * up, 1.0f);
    vertices[1] = float4(input[0].position + halfWidth * right + halfHeight * up, 1.0f);
    vertices[2] = float4(input[0].position - halfWidth * right - halfHeight * up, 1.0f);
    vertices[3] = float4(input[0].position - halfWidth * right + halfHeight * up, 1.0f);
    
    PixelInput output;
    
    [unroll]
    for (int i = 0; i < 4; i++)
    {
        output.position = mul(vertices[i], view);
        
        output.alpha = saturate(1 - output.position.z / input[0].distance) * 0.5f;
        
        output.position = mul(output.position, projection);
        
        output.uv = TEXCOORD[i];
        output.color = input[0].color;        
        
        stream.Append(output);
    }

}

float4 PS(PixelInput input) : SV_TARGET
{
    float4 result = map.Sample(linearSamp, input.uv);
    
    result.rgb = input.color.rgb * (1.0f - input.alpha) * 2.0f;
    result.a = result.a * (1.0f - input.alpha) * 1.5f;
    
    return result;
}