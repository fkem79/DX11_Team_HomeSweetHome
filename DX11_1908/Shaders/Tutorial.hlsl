//Tutorial
cbuffer VP : register(b0)
{    
    matrix view;
    matrix projection;
}

cbuffer W : register(b1)
{
    matrix world;    
}

cbuffer Color : register(b0)
{
    float4 color;
}



struct VertexInput
{
    float4 position : POSITION;     
};

struct PixelInput
{
    float4 position : SV_Position;     
};

PixelInput VS( VertexInput input )
{
    PixelInput output;
    output.position = mul(input.position, world);
    output.position = mul(output.position, view);
    output.position = mul(output.position, projection);    
    
	return output;
}

float4 PS(PixelInput input) : SV_TARGET
{
    return color;
}