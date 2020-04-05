//GeometryBillboard.hlsl
#include "Header.hlsli"

Texture2D map : register(t10);

struct VertexOutput
{
    float3 position : POSITION;
    float2 size : SIZE;
};

VertexOutput VS(VertexSize input)
{
    VertexOutput output;
    output.position = input.position.xyz;
    output.size = input.size;
    
    return output;
}

struct PixelInput
{
    float4 position : SV_Position;
    float2 uv : UV;    
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
    float3 up = float3(0, 1, 0);
    float3 forward = CamPos() - input[0].position;
    forward.y = 0.0f;
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
        output.position = mul(output.position, projection);
        
        output.uv = TEXCOORD[i];
        
        stream.Append(output);
    }

}

float4 PS(PixelInput input) : SV_TARGET
{
    return map.Sample(linearSamp, input.uv);
}