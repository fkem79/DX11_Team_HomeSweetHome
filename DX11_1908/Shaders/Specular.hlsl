//Specular
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

cbuffer Light : register(b0)
{
    float3 lightDir;
    
    float specExp;
}

Texture2D diffuseMap : register(t0);
SamplerState diffuseSamp : register(s0);

Texture2D specularMap : register(t1);
SamplerState specularSamp : register(s1);

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
    float3 viewDir : VIEWDIR;
};

PixelInput VS(VertexInput input)
{
    PixelInput output;
    output.position = mul(input.position, world);
    
    output.viewDir = normalize(output.position.xyz - invView._41_42_43);
    
    output.position = mul(output.position, view);
    output.position = mul(output.position, projection);
    
    output.uv = input.uv;
    
    output.normal = mul(input.normal, (float3x3) world);
    
    return output;
}

float4 PS(PixelInput input) : SV_TARGET
{
    float4 albedo = diffuseMap.Sample(diffuseSamp, input.uv);
    
    float3 light = normalize(lightDir);
    float3 normal = normalize(input.normal);
    
    float diffuse = saturate(dot(normal, -light));
    
    float4 specular = 0;
    if(diffuse > 0)
    {
        //Phong Shading
        //float3 reflection = normalize(reflect(light, normal));
        //specular = saturate(dot(reflection, -input.viewDir));
        
        //Blinn Phong Shading
        float3 halfWay = normalize(input.viewDir + light);
        specular = saturate(dot(-halfWay, normal));
        
        float4 specularIntensity = specularMap.Sample(specularSamp, input.uv);
        
        specular = pow(specular, specExp) * specularIntensity;
    }
    
    float4 ambient = albedo * 0.1f;
    
    return albedo * diffuse + specular + ambient;
}