#include "Header.hlsli"

cbuffer Data : register(b10)
{
    float3 velocity;
    float drawDistance;
    
    float4 color;
    float3 origin;
    float time;
    
    float3 size;
}

Texture2D map : register(t10);

struct VertexOutput
{
    float3 position : POSITION;
    float2 size : SIZE;
	float2 distance : DISTANCE;
	float4 color : COLOR;
	float3 velocity : VELOCITY;
};

VertexOutput VS(VertexSize input)
{
    VertexOutput output;
    
	float3 v = velocity;
	v.xz /= input.size.y * 0.1f;
	output.velocity = v;
    
	float3 displace = time * v;
	input.position.xyz = origin.xyz +
    (size.xyz + (input.position.xyz + displace.xyz) % size.xyz) % size.xyz - (size.xyz * 0.5f);
    
	float alpha = cos(time + (input.position.x + input.position.z));
	alpha = saturate(1.5f + alpha / drawDistance * 2);
    
	output.distance.x = drawDistance;
	output.distance.y = alpha;
    
	output.color = color;
    
	output.position = input.position.xyz;
	output.size = input.size;
    
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
        output.position = mul(output.position, projection);
        
        output.uv = TEXCOORD[i];
		output.color = input[0].color;
        
		output.alpha = 0.2f * saturate(1 - output.position.z / input[0].distance.x)
        * input[0].distance.y;
        
        stream.Append(output);
    }

}

float4 PS(PixelInput input) : SV_TARGET
{
	float4 result = map.Sample(linearSamp, input.uv);
    
	result.rgb *= input.color * (1 + input.alpha) * 2.0f;
    
	result.a *= input.alpha;
    
	return result;
}