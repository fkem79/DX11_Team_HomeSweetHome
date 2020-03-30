#include "Header.hlsli"

cbuffer Spark : register(b10)
{
	float duration;
	float time;
	float gravity;
}

cbuffer Color : register(b10)
{
	float4 startColor;
	float4 endColor;
}

Texture2D map : register(t10);

struct VertexInput
{
	float4 position : POSITION;
	float2 size : SIZE;
	float3 velocity : VELOCITY;
};

struct VertexOutput
{
	float3 position : POSITION;
	float2 size : SIZE;	
	float time : TIME;
};

VertexOutput VS(VertexInput input)
{
	VertexOutput output;
	
	output.time = time / duration;
	
	input.velocity.y -= gravity * time;
	
	output.position = input.position.xyz + input.velocity * time;
	output.position = mul(output.position, (float3x3) world);
	
	output.size = input.size;
    
	return	output;
}

struct PixelInput
{
	float4 position : SV_Position;
	float2 uv : UV;
	float time : TIME;
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
	float3 up = invView._21_22_23;
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
    
	output.time = input[0].time;
	
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
	float4 result = map.Sample(linearSamp, input.uv);
	
	float4 color = lerp(startColor, endColor, input.time);
	
	return result * color;
}