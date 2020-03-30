cbuffer VP : register(b0)
{
    matrix view;
    matrix projection;
}

cbuffer W : register(b1)
{
    matrix world;
}

cbuffer Light : register(b2)
{
    float3 lightPos;
}

cbuffer Color : register(b0)
{
    float4 color;
}

Texture2D map : register(t0);
SamplerState samp : register(s0);

struct VertexInput
{
    float4 position : POSITION;
    float2 uv : UV;
    float3 normal : NORMAL;
};

struct PixelInput
{
    float4 position : SV_Position;
    float diffuse : DIFFUSE;
    float2 uv : UV;
};

PixelInput VS(VertexInput input)
{
    PixelInput output;
    output.position = mul(input.position, world);
    
    float3 light = normalize(output.position.xyz - lightPos);
    
    output.position = mul(output.position, view);
    output.position = mul(output.position, projection);
    
    float3 normal = normalize(mul(input.normal, (float3x3)world));    
    
    output.diffuse = dot(normal, -light);
    
    output.uv = input.uv;
    
    return output;
}

float4 PS(PixelInput input) : SV_TARGET
{
    return map.Sample(samp, input.uv) * color * input.diffuse;
}