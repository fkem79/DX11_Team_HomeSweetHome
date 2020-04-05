//Grid
cbuffer VP : register(b0)
{
    matrix view;
    matrix projection;
}

cbuffer W : register(b1)
{
    matrix world;
}

cbuffer Light : register(b0)
{
    float3 lightDir;
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
    float2 uv : UV;
    float3 normal : NORMAL;    
};

PixelInput VS(VertexInput input)
{
    PixelInput output;
    output.position = mul(input.position, world);
    output.position = mul(output.position, view);
    output.position = mul(output.position, projection);
    
    output.uv = input.uv;
    
    output.normal = mul(input.normal, (float3x3) world);
    
    return output;
}

float4 PS(PixelInput input) : SV_TARGET
{
    float4 albedo = map.Sample(samp, input.uv);
    
    float3 light = normalize(lightDir);
    float3 normal = normalize(input.normal);
    
    float diffuse = saturate(dot(normal, -light));
    
    
    return albedo * diffuse;
}