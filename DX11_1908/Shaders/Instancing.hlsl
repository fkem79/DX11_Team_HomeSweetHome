#include "Header.hlsli"

struct VertexInput
{
    float4 position : POSITION;
    float2 uv : UV;
    
    matrix transform : INSTANCE;
    uint instanceID : SV_InstanceID;
};

struct PixelInput
{
    float4 position : SV_Position;
    float2 uv : UV;
    float4 color : COLOR;
};

PixelInput VS(VertexInput input)
{
    PixelInput output;
    output.position = mul(input.position, input.transform);
    output.position = mul(output.position, view);
    output.position = mul(output.position, projection);
    
    output.uv = input.uv;
    
    
    if(input.instanceID % 3 == 0)
    {
        output.color = float4(1, 0, 0, 1);
    }else if(input.instanceID % 3 == 1)
    {
        output.color = float4(0, 1, 0, 1);
    }
    else if (input.instanceID % 3 == 2)
    {
        output.color = float4(0, 0, 1, 1);
    }
    
    return output;
}

float4 PS(PixelInput input) : SV_TARGET
{
    return diffuseMap.Sample(linearSamp, input.uv) * input.color;
}